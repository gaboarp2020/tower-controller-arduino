#ifndef Screen_h
#define Screen_h

#include <Wire.h>
#include "SSD1306Wire.h"

#define ADRESS 0x3C
#define SDA 16
#define SCL 17

SSD1306Wire _display = SSD1306Wire(ADRESS, SDA, SCL);

unsigned int keyval=0;

class Screen
{
public:
    Screen();
    void begin();
    void clear(void);
    void display(void);
    void drawCircle(int16_t x, int16_t y, int16_t radius);
    void drawRect(int16_t x, int16_t y, int16_t width, int16_t height);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void fillCircle(int16_t x, int16_t y, int16_t radius);
    void fillRect(int16_t x, int16_t y, int16_t width, int16_t height);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    void invertDisplay(void);
    void normalDisplay(void);
    void text(int16_t x, int16_t y, const String &text);void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

private:
};

Screen::Screen()
{
}

void Screen::begin()
{
    _display.init();
    _display.flipScreenVertically();
    _display.setFont(ArialMT_Plain_10);
}

void Screen::clear()
{
    _display.clear();
}

void Screen::display()
{
    _display.display();
}

void Screen::text(int16_t x, int16_t y, const String &text)
{
    _display.drawString(x, y, text);
}

void Screen::invertDisplay(void)
{
    _display.invertDisplay();
}

void Screen::normalDisplay(void)
{
    _display.normalDisplay();
}
void Screen::drawCircle(int16_t x, int16_t y, int16_t radius)
{
    _display.drawCircle(x, y, radius);
}
void Screen::drawRect(int16_t x, int16_t y, int16_t width, int16_t height)
{
    _display.drawRect(x, y, width, height);
}
void Screen::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    _display.drawTriangle(x0, y0, x1, y1, x2, y2);
}
void Screen::fillCircle(int16_t x, int16_t y, int16_t radius)
{
    _display.fillCircle(x, y, radius);
}
void Screen::fillRect(int16_t x, int16_t y, int16_t width, int16_t height)
{
    _display.fillRect(x, y, width, height);
}
void Screen::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    _display.fillTriangle(x0, y0, x1, y1, x2, y2);
}

#endif
