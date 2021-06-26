#ifndef Connection_h
#define Connection_h

#include <WiFi.h>
#include <HTTPClient.h>

#include "Config.h"
#include "Serial.h"
#include "Utils.h"

// Config Preferences Scope
#define CONFIG_SCOPE "Config"

// WiFi Default Credentials
#define DEFAULT_SSID "Tower Control"
#define DEFAULT_PASS "1234567"

// Model Default (AP | CLIENT)
#define DEFAULT_MODE "AP"

#define LOOP_INTERVAL 10000

Config config(CONFIG_SCOPE);

class Connection
{
public:
    void begin();
    // Mode
    String getMode();
    bool setMode(String mode);
    bool isMode(String mode);
    bool isValidMode(String mode);
    // Access Point
    bool setAPConfig(String ssid, String pass);
    // Utils
    void clearConfig();
    bool isConnected();

private:
    // Connection state handle flags
    unsigned long _previousMillis;
    unsigned long _currentMillis;
    // Internal methods
    void beginAP();
    bool isNetworkStatus(int status);
};

String Connection::getMode()
{
    return config.get("mode", DEFAULT_MODE);
}

bool Connection::setMode(String mode)
{
    consoleLog("Trying to save mode config");
    consoleLog("Mode: " + mode);

    if (Connection::isValidMode(mode))
    {
        config.set("mode", mode);

        return true;
    }

    return false;
}

bool Connection::isMode(String mode)
{
    return mode == Connection::getMode();
}

bool Connection::isValidMode(String mode)
{
    return mode == "AP" || mode == "CLIENT";
}

bool Connection::setAPConfig(String ssid, String password)
{
    consoleLog("Trying to save access point config");
    consoleLog("SSID: " + ssid);
    consoleLog("PASS: " + password);

    if (isValidWiFiSSID(ssid) && isValidWiFiPassword(password))
    {
        config.set("ap_ssid", ssid);
        config.set("ap_password", password);

        return true;
    }

    return false;
}

void Connection::clearConfig()
{
    config.clear();
}

void Connection::begin()
{
    String mode = Connection::getMode();

    consoleLog("Mode: " + mode);

    if (mode == "AP")
    {
        Connection::beginAP();
    }
}

void Connection::beginAP()
{
    WiFi.mode(WIFI_AP);

    consoleLog("Creating WiFi Access Point");

    const String wnet = config.get("ap_ssid", DEFAULT_SSID);
    const String passwd = config.get("ap_password", DEFAULT_PASS);

    if (isValidWiFiSSID(wnet))
    {
        consoleLog("SSID: " + wnet);

        if (isValidWiFiPassword(passwd))
        {
            consoleLog("PASS: " + passwd);

            WiFi.softAP(wnet.c_str(), passwd.c_str());
        }
        else
        {
            WiFi.softAP(wnet.c_str());
        }
    }

    consoleLog("IP address: " + IPAddressToString(WiFi.softAPIP()));
}

bool Connection::isNetworkStatus(int status)
{
    // WiFi.status()
    //
    // 3   WL_CONNECTED: assigned when connected to a WiFi network;
    // 255 WL_NO_SHIELD: assigned when no WiFi shield is present;
    // 0   WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains
    //      active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a
    //      connection is established (resulting in WL_CONNECTED);
    // 1   WL_NO_SSID_AVAIL: assigned when no SSID are available;
    // 2   WL_SCAN_COMPLETED: assigned when the scan networks is completed;
    // 4   WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
    // 5   WL_CONNECTION_LOST: assigned when the connection is lost;
    // 6   WL_DISCONNECTED: assigned when disconnected from a network;
    //
    // Take a look at the WiFi possible status code at https://www.arduino.cc/en/Reference/WiFiStatus
    return status == WiFi.status();
}

#endif