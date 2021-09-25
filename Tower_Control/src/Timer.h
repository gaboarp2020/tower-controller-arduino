#ifndef Timer_h
#define Timer_h

class Timer
{
public:
    Timer(int timerControlPin, unsigned long controlMillis, bool relayNO);
    void begin();
    void loop();
    void start();
    void stop();

private:
    unsigned long _currentMillis;
    unsigned long _controlMillis;
    unsigned long _startMillis;
    int _timerControlPin;
    bool _isActive;
    bool _relayNO;
    void setup();
    
};

Timer::Timer(int timerControlPin, unsigned long controlMillis, bool relayNO)
{
    // Timer Control Milliseconds
    _controlMillis = controlMillis;
    _currentMillis = 0;
    _startMillis = 0;
    _isActive = false;

    // Timer Control Pin
    _timerControlPin = timerControlPin;

    // Relay Mode
    _relayNO = relayNO;
}

void Timer::begin()
{
    this->setup();
}

void Timer::loop()
{
    if (_isActive)
    {
        _currentMillis = millis();

        if (_currentMillis - _startMillis >= _controlMillis)
        {
            digitalWrite(_timerControlPin, _relayNO ? HIGH : LOW);
            _startMillis = _currentMillis;
            _isActive = false;
        }
    }
}

void Timer::setup()
{
    pinMode(_timerControlPin, OUTPUT);
}

void Timer::start()
{
    if (!_isActive)
    {
        _isActive = true;
        _startMillis = millis();
        digitalWrite(_timerControlPin, _relayNO ? LOW : HIGH);
    }
}

void Timer::stop()
{
    if (_isActive)
    {
        digitalWrite(_timerControlPin, _relayNO ? HIGH : LOW);
        _startMillis = _currentMillis;
        _isActive = false;
    }
}

#endif
