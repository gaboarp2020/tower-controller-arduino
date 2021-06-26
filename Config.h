#ifndef Config_h
#define Config_h

#include <Preferences.h>

#define WAIT 100

class Config
{
  public:
    Config(const char* scope);
    String get(const char* key, const char* defaultValue);
    String get(String key, const char* defaultValue);
    void set(const char* key, const char* value);
    void set(const char* key, String value);
    void set(String key, String value);
    void clear();
  private:
    const char* _scope;
    static String getByScope(const char* scope, const char* key, const char* defaultValue);
    static void setByScope(const char* scope, const char* key, const char* value);
    static void clearByScope(const char* scope);
};

Config::Config(const char* scope) {
  _scope = scope;
}

String Config::getByScope(const char* scope, const char* key, const char* defaultValue = "")
{
  Preferences preferences;
  preferences.begin(scope, true);
  String value = preferences.getString(key, defaultValue);
  preferences.end();

  return value;
}

void Config::setByScope(const char* scope, const char* key, const char* value)
{
  Preferences preferences;
  preferences.begin(scope, false);
  preferences.putString(key, value);
  preferences.end();

  delay(WAIT);
}

void Config::clearByScope(const char* scope)
{
  Preferences preferences;
  preferences.begin(scope, false);
  preferences.clear();
  preferences.end();

  delay(WAIT);
}

String Config::get(const char* key, const char* defaultValue = "")
{
  Preferences preferences;
  preferences.begin(_scope, true);
  String value = preferences.getString(key, defaultValue);
  preferences.end();

  return value;
}

String Config::get(String key, const char* defaultValue = "")
{
  Preferences preferences;
  preferences.begin(_scope, true);
  String value = preferences.getString(key.c_str(), defaultValue);
  preferences.end();

  return value;
}

void Config::set(const char* key, const char* value)
{
  Preferences preferences;
  preferences.begin(_scope, false);
  preferences.putString(key, value);
  preferences.end();

  delay(WAIT);
}

void Config::set(const char* key, String value)
{
  Preferences preferences;
  preferences.begin(_scope, false);
  preferences.putString(key, value);
  preferences.end();

  delay(WAIT);
}

void Config::set(String key, String value)
{
  Preferences preferences;
  preferences.begin(_scope, false);
  preferences.putString(key.c_str(), value);
  preferences.end();

  delay(WAIT);
}

void Config::clear()
{
  Preferences preferences;
  preferences.begin(_scope, false);
  preferences.clear();
  preferences.end();

  delay(WAIT);
}

#endif