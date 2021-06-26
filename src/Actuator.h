#ifndef Actuator_h
#define Actuator_h

#define ACTION_ELEVATION 1
#define ACTION_INCLINATION 2

#define DIRECTION_UP 1
#define DIRECTION_DOWN 2

class Actuator
{
public:
    Actuator(int elevationUpPin, int elevationDownPin, int inclinationUpPin, int inclinationDownPin);
    void begin();
    bool elevate(int direction);
    bool inclinate(int direction);
private:
    int _elevationDownPin;
    int _elevationUpPin;
    int _inclinationDownPin;
    int _inclinationUpPin;
    // Internal Methods
    void setup();
    void stop();
    bool move(int action, int direction);
    bool isValidAction(int action);
    bool isValidDirection(int direction);
    int getPinByActionAndDirection(int action, int direction);
}

Actuator::Actuator(int elevationUpPin, int elevationDownPin, int inclinationUpPin, int inclinationDownPin)
{
    // Elevation Pins
    _elevationDownPin = elevationDownPin;
    _elevationUpPin = elevationUpPin;

    // Inclination Pins
    _inclinationDownPin = inclinationDownPin;
    _inclinationUpPin = inclinationUpPin;
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

// Internal Methods

void Actuator::setup()
{
    pinMode(_elevationDownPin, OUTPUT);
    pinMode(_elevationUpPin, OUTPUT);

    pinMode(_inclinationDownPin, OUTPUT);
    pinMode(_inclinationUpPin, OUTPUT);
}

void Actuator::stop()
{
    digitalWrite(_elevationDownPin, LOW);
    digitalWrite(_elevationUpPin, LOW);

    digitalWrite(_inclinationDownPin, LOW);
    digitalWrite(_inclinationUpPin, LOW);
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

    digitalWrite(pin, HIGH);

    return true;
}

bool Actuator::isValidAction(int action)
{
    return action == ACTION_ELEVATION || action === ACTION_INCLINATION;
}

bool Actuator::isValidDirection(int direction)
{
    return direction == DIRECTION_DOWN || direction == DIRECTION_UP;
}

int Actuator::getPinByActionAndDirection(int action, int direction)
{
    if (action == ACTION_ELEVATION) 
    {
        if (direction == DIRECTION_DOWN)
        {
            return _elevationDownPin;
        }
        else if (direction == DIRECTION_UP)
        {
            return _elevationUpPin;
        }
    }
    else if (action == ACTION_INCLINATION)
    {
        if (direction == DIRECTION_DOWN)
        {
            return _inclinationDownPin;
        }
        else if (direction == DIRECTION_UP)
        {
            return _inclinationUpPin;
        }
    }

    return 0;
}

#endif