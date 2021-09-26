#ifndef Utils_h
#define Utils_h

#include <FunctionalInterrupt.h>
#include <MD5.h>

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

String getChipId()
{
  uint32_t chipId = 0;

  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  return String(chipId);
}

String stringToMD5(String v)
{

  // Length (with one extra character for the null terminator)
  int str_len = v.length() + 1;
  // Prepare the character array (the buffer)
  char value[str_len];
  // Copy it over
  v.toCharArray(value, str_len);

  unsigned char *hash = MD5::make_hash(value);
  //generate the digest (hex encoding) of our hash
  char *md5str = MD5::make_digest(hash, 16);
  free(hash);

  String result = String(md5str);

  return result;
}

#endif