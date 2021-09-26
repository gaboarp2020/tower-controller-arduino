#ifndef Software_Clone_h
#define Software_Clone_h

#include <FunctionalInterrupt.h>

#include "Utils.h"

class SoftwareClone
{
public:
  SoftwareClone(String key);
  boolean isCloned();
private:
  String _keyMD5;
  String _chipId;
  String _chipIdMD5;
};

SoftwareClone::SoftwareClone(String key)
{
  _keyMD5 = key;

  _chipId = getChipId();
  _chipIdMD5 = stringToMD5(_chipId);
}

boolean SoftwareClone::isCloned() {
  return _keyMD5 != _chipIdMD5;
}

#endif
