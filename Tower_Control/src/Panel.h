#ifndef Panel_h
#define Panel_h

#include "Router.h"

Router router;

#include "Screen.h"
#include "Serial.h"
#include "Utils.h"

#ifndef VERSION
#define VERSION "v1.0.0"
#endif

#ifndef SCREEN_FORCE_REFRESH_INTERVAL
#define SCREEN_FORCE_REFRESH_INTERVAL 1000
#endif

#ifndef SCREEN_REFRESH_INTERVAL
#define SCREEN_REFRESH_INTERVAL 17 // ~ 1000 / 60
#endif

#ifndef SCREEN_INPUT_BLINK_INTERVAL
#define SCREEN_INPUT_BLINK_INTERVAL 700
#endif

#ifndef SCREEN_INACTIVITY_REDIRECT_INTERVAL
#define SCREEN_INACTIVITY_REDIRECT_INTERVAL 30000
#endif

#ifndef PANEL_REBOOT_ATTEMPTS
#define PANEL_REBOOT_ATTEMPTS 5
#endif

#define TEXT_FAN_STATUS_LENGTH 3
#define TEXT_DATA_LENGTH 6
#define TEXT_INPUT_LENGTH 2

class Panel
{
public:
    Panel();
    void bootPage();
    void displayUpArrow();
    void displayDownArrow();
    void displayFrontArrow();
    void displayBackArrow();
    void displayStop();
    void elevateUpPage();
    void elevateDownPage();
    void inclinateUpPage();
    void inclinateDownPage();
    void mainPage();
    void stopPage();
    void begin();
    void loop();
    void navigate(String page);

private:
    Screen *_screen;
    // Control flags
    unsigned long _currentMillis;
    unsigned long _previousBlinkMillis;
    unsigned long _previousForceRefreshMillis;
    unsigned long _previousInactivityRedirectMillis;
    unsigned long _previousRefreshMillis;
    bool _shouldRefresh;
    String _renderedPage;
    bool _partialRender;
    int _fieldIndex;
    bool _blink;
    int _rebootAttempts;
    // Internal Methods
    void render();
    void forceRender();
    void toggleBlink();
    void onExitSensorConfig();
    void trackActivity();
    void inactivityRedirect();
    void panelHeader();
};

Panel::Panel()
{
    _screen = new Screen();

    _previousBlinkMillis = 0;
    _previousForceRefreshMillis = 0;
    _previousInactivityRedirectMillis = 0;
    _previousRefreshMillis = 0;

    _partialRender = false;

    _rebootAttempts = 0;

    this->navigate(PAGE_BOOT);
}

void Panel::render()
{
    if (!_shouldRefresh)
    {
        return;
    }

    _shouldRefresh = false;

    if (router.current() == PAGE_BOOT)
    {
        this->bootPage();
    }
    else if (router.current() == PAGE_ELEVATE_UP)
    {
        this->elevateUpPage();
    }
    else if (router.current() == PAGE_ELEVATE_DOWN)
    {
        this->elevateDownPage();
    }
    else if (router.current() == PAGE_INCLINATE_UP)
    {
        this->inclinateUpPage();
    }
    else if (router.current() == PAGE_INCLINATE_DOWN)
    {
        this->inclinateDownPage();
    }
    else if (router.current() == PAGE_STOP)
    {
        this->stopPage();
    }
    else if (router.current() == PAGE_MAIN)
    {
        this->mainPage();
    }

    _partialRender = router.current() == _renderedPage;
    _renderedPage = router.current();
}

void Panel::forceRender()
{
    _shouldRefresh = true;
}

void Panel::toggleBlink()
{
    _blink = !_blink;
    this->forceRender();
}

void Panel::trackActivity()
{
    _previousInactivityRedirectMillis = millis();
}

void Panel::inactivityRedirect()
{
    this->trackActivity();

    if (
            router.current() == PAGE_BOOT
            || router.current() == PAGE_ELEVATE_UP
            || router.current() == PAGE_ELEVATE_DOWN
            || router.current() == PAGE_INCLINATE_UP
            || router.current() == PAGE_INCLINATE_DOWN
            || router.current() == PAGE_STOP
            || router.current() == PAGE_MAIN
        )
    {
        return;
    }

    return this->navigate(PAGE_MAIN);
}

void Panel::bootPage()
{
    if (_partialRender)
    {
        return;
    }    
    
    _screen->clear();
    this->panelHeader();
    _screen->display();

    delay(5000);

    _screen->clear();
    _screen->text(0, 0, "Initialization...");
    _screen->display();

    delay(5000);

    this->navigate(PAGE_MAIN);
}

void Panel::mainPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->normalDisplay();
        _screen->text(0, 0, "Tower at rest");
        _screen->display();
    }
}

void Panel::elevateUpPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->invertDisplay();
        this->displayUpArrow();
        _screen->text(56, 45, "UP");
        _screen->display();
    }
}

void Panel::elevateDownPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->invertDisplay();
        this->displayDownArrow();
        _screen->text(52, 45, "DOWN");
        _screen->display();
    }
}

void Panel::inclinateUpPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->invertDisplay();
        this->displayFrontArrow();
        _screen->text(50, 45, "FRONT");
        _screen->display();
    }
}

void Panel::inclinateDownPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->invertDisplay();
        this->displayBackArrow();
        _screen->text(50, 45, "BACK");
        _screen->display();
    }
}

void Panel::stopPage()
{
    if (!_partialRender)
    {
        _screen->clear();
        _screen->normalDisplay();
        this->displayStop();
        _screen->text(52, 45, "STOP");
        _screen->display();
    }
}

void Panel::begin()
{
    _screen->begin();
}

void Panel::loop()
{
    _currentMillis = millis();

    if (_currentMillis - _previousBlinkMillis >= SCREEN_INPUT_BLINK_INTERVAL)
    {
        _previousBlinkMillis = _currentMillis;
        this->toggleBlink();
    }

    if (_shouldRefresh || _currentMillis - _previousRefreshMillis >= SCREEN_REFRESH_INTERVAL)
    {
        _previousRefreshMillis = _currentMillis;
        this->render();
    }

    if (_currentMillis - _previousForceRefreshMillis >= SCREEN_FORCE_REFRESH_INTERVAL)
    {
        _previousForceRefreshMillis = _currentMillis;
        this->forceRender();
    }

    if (_currentMillis - _previousInactivityRedirectMillis >= SCREEN_INACTIVITY_REDIRECT_INTERVAL)
    {
        _previousInactivityRedirectMillis = _currentMillis;
        this->inactivityRedirect();
    }
}

void Panel::navigate(String page)
{
    router.goTo(page);

    _partialRender = false;

    this->forceRender();
}

void Panel::panelHeader()
{
    _screen->text(0, 0, "designed for");
    _screen->text(0, 15, "NOKIA");
    _screen->text(0, 30, "Tower Control");
    _screen->text(0, 45, VERSION);
}

// Display Arrows functions
void Panel::displayUpArrow()
{
    _screen->fillRect(60, 20, 8, 20);
    _screen->fillTriangle(56, 20, 64, 5, 72, 20);
}

void Panel::displayDownArrow()
{
    _screen->fillRect(60, 5, 8, 20);
    _screen->fillTriangle(56, 25, 64, 40, 72, 25);
}

void Panel::displayFrontArrow()
{
    _screen->fillRect(54, 15, 13, 8);
    _screen->fillRect(49, 19, 8, 20);
    _screen->fillCircle(53, 19, 4);
    _screen->fillTriangle(66, 11, 81, 19, 66, 27);
}

void Panel::displayBackArrow()
{
    _screen->fillRect(51, 13, 8, 13);
    _screen->fillRect(55, 9, 20, 8);
    _screen->fillCircle(55, 13, 4);
    _screen->fillTriangle(47, 26, 55, 41, 64, 26);
}

void Panel::displayStop()
{
    _screen->fillRect(55, 14, 8, 20);
    _screen->fillRect(66, 14, 8, 20);
}

#endif
