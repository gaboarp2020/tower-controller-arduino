#ifndef Serial_h
#define Serial_h

#ifdef DEBUG

bool isValidSerialPort(int port)
{
  return port == 9600 || port == 115200;
}

void setupSerial(int port)
{
  if (!isValidSerialPort(port))
  {
    return;
  }

  // Open serial communications and wait for port to open
  Serial.begin(port);
  while (!Serial)
  {
  }

  delay(1000);
}

void consoleLog(String message)
{
  Serial.println(message);
}

#else

void setupSerial(int port) {}

void consoleLog(String message) {}

#endif

#endif