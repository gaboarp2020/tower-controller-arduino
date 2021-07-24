#ifndef Actuator_h
#define Actuator_h

#define ACTION_ELEVATION 1
#define ACTION_INCLINATION 2

#define DIRECTION_UP 1
#define DIRECTION_DOWN 2

class Actuator
{
public:
    Actuator(int elevationUpPin, int elevationDownPin, int inclinationUpPin, int inclinationDownPin, bool relayNO);
    void begin();
    bool elevate(int direction);
    bool inclinate(int direction);
    void stop();

private:
    int _elevationDownPin;
    int _elevationUpPin;
    int _inclinationDownPin;
    int _inclinationUpPin;
    bool _relayNO;
    // Internal Methods
    void setup();
    bool move(int action, int direction);
    bool isValidAction(int action);
    bool isValidDirection(int direction);
    int getPinByActionAndDirection(int action, int direction);
};

Actuator::Actuator(int elevationUpPin, int elevationDownPin, int inclinationUpPin, int inclinationDownPin, bool relayNO)
{
    // Elevation Pins
    _elevationDownPin = elevationDownPin;
    _elevationUpPin = elevationUpPin;

    // Inclination Pins
    _inclinationDownPin = inclinationDownPin;
    _inclinationUpPin = inclinationUpPin;

    // Relay Mode
    _relayNO = relayNO;
}

// Public Methods

void Actuator::begin()
{
    this->setup();
    this->stop();
}

bool Actuator::elevate(int direction)
{
    if (!this->isValidDirection(direction))
    {
        return false;
    }

    return this->move(ACTION_ELEVATION, direction);
}

bool Actuator::inclinate(int direction)
{
    if (!this->isValidDirection(direction))
    {
        return false;
    }

    return this->move(ACTION_INCLINATION, direction);
}

void Actuator::stop()
{
    digitalWrite(_elevationDownPin, _relayNO ? HIGH : LOW);
    digitalWrite(_elevationUpPin, _relayNO ? HIGH : LOW);

    digitalWrite(_inclinationDownPin, _relayNO ? HIGH : LOW);
    digitalWrite(_inclinationUpPin, _relayNO ? HIGH : LOW);
}

// Internal Methods

void Actuator::setup()
{
    pinMode(_elevationDownPin, OUTPUT);
    pinMode(_elevationUpPin, OUTPUT);

    pinMode(_inclinationDownPin, OUTPUT);
    pinMode(_inclinationUpPin, OUTPUT);
}

bool Actuator::move(int action, int direction)
{
    this->stop();

    if (!this->isValidAction(action) || !this->isValidDirection(direction))
    {
        return false;
    }

    int pin = this->getPinByActionAndDirection(action, direction);

    if (pin == 0)
    {
        return false;
    }

    digitalWrite(pin, _relayNO ? LOW : HIGH);

    return true;
}

bool Actuator::isValidAction(int action)
{
    return action == ACTION_ELEVATION || action == ACTION_INCLINATION;
}

bool Actuator::isValidDirection(int direction)
{
    return direction == DIRECTION_DOWN || direction == DIRECTION_UP;
}

int Actuator::getPinByActionAndDirection(int action, int direction)
{
    // Elevation
    if (action == ACTION_ELEVATION)
    {
        // Elevation Down
        if (direction == DIRECTION_DOWN)
        {
            return _elevationDownPin;
        }

        // Elevation Up
        if (direction == DIRECTION_UP)
        {
            return _elevationUpPin;
        }
    }

    // Inclination
    if (action == ACTION_INCLINATION)
    {
        // Inclination Down
        if (direction == DIRECTION_DOWN)
        {
            return _inclinationDownPin;
        }

        // Inclination Up
        if (direction == DIRECTION_UP)
        {
            return _inclinationUpPin;
        }
    }

    return 0;
}

#endif