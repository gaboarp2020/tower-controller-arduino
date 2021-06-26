#ifndef Utils_h
#define Utils_h

#include <FunctionalInterrupt.h>

String charToString(const char C[])
{
  byte at = 0;
  const char *p = C;
  String S = "";

  while (*p++)
  {
    S.concat(C[at++]);
  }

  return S;
}

void stringToCharArray(String str, char *buff)
{
  str.toCharArray(buff, str.length() + 1);
}

String IPAddressToString(const IPAddress &ipAddress)
{
  return String(ipAddress[0]) + "." + String(ipAddress[1]) + "." + String(ipAddress[2]) + "." + String(ipAddress[3]);
}

bool isPinActive(int pin)
{
  int state = digitalRead(pin);

  return state == HIGH;
}

void restart()
{
  delay(100);
  ESP.restart();
}

bool isValidWiFiSSID(String ssid)
{
  int length = ssid.length();

  return length > 0 && length < 31;
}

bool isValidWiFiPassword(String password)
{
  int length = password.length();

  return length > 7 && length < 64;
}

#endif