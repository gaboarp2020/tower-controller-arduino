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

WiFiClient wifiClient;

// We handle a logical status of connection to avoid requesting this info to WiFi
// because this will be implemented in a highly called function
bool isConnectedToWiFi = false;

class Connection
{
public:
    // Mode
    String getMode();
    bool setMode(String mode);
    bool isMode(String mode);
    bool isValidMode(String mode);
    // Access Point
    bool setAPConfig(String ssid, String pass);
    // Client
    bool setClientConfig(String ssid, String pass);
    // Hash
    String getHash();
    bool setHash(String hash);
    bool isValidHash(String hash);
    // Utils
    void begin();
    bool connectToNetwork(String wnet, String passwd, bool retry);
    void clearConfig();
    bool isConnected();

private:
    // Connection state handle flags
    unsigned long _previousMillis;
    unsigned long _currentMillis;
    // Internal methods
    void beginAP();
    void beginClient();
    bool isNetworkStatus(int status);
    bool connectToServer();
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

bool Connection::setClientConfig(String ssid, String password)
{
    consoleLog("Trying to save client config");
    consoleLog("SSID: " + ssid);
    consoleLog("PASS: " + password);

    if (isValidWiFiSSID(ssid))
    {
        config.set("client_ssid", ssid);

        if (isValidWiFiPassword(password))
        {
            config.set("client_password", password);
        }

        return true;
    }

    return false;
}

void Connection::clearConfig()
{
    config.clear();
}

bool Connection::isConnected()
{  
    return isConnectedToWiFi && isConnectedToWebSocket;
}

String Connection::getHash()
{
    return config.get("hash", "");
}

bool Connection::setHash(String hash)
{
    consoleLog("Trying to save hash " + hash);

    if (Connection::isValidHash(hash))
    {
        config.set("hash", hash);

        return true;
    }

    return false;
}

bool Connection::isValidHash(String hash)
{
    int length = hash.length();

    return length > 0;
}

void Connection::begin()
{
    String mode = Connection::getMode();

    consoleLog("Mode: " + mode);

    if (mode == "AP")
    {
        Connection::beginAP();
    }
    else if (mode == "CLIENT")
    {
        Connection::beginClient();
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

// void Connection::beginClient()
// {
//     WiFi.mode(WIFI_STA);

//     consoleLog("Setting WiFi Client");

//     const String wnet = config.get("client_ssid", "");
//     const String passwd = config.get("client_password", "");

//     if (Connection::connectToNetwork(wnet, passwd, true))
//     {
//         consoleLog("WiFi connected successfully");
//         consoleLog("IP address: " + IPAddressToString(WiFi.localIP()));

//         isConnectedToWiFi = true;

//         if (Connection::connectToServer())
//         {
//             consoleLog("Server connected successfully");
          
//             const String hash = Connection::getHash();
//             consoleLog("Hash: " + hash);

//             if (Connection::isValidHash(hash))
//             {
//                 if (Connection::connectToWebsocketServer())
//                 {
//                     consoleLog("Successfully pair with websocket server");

//                     isConnectedToWebSocket = true;
//                 }
//                 else
//                 {
//                     consoleLog("Failed pairing with websocket server");
//                     // TODO: We should reboot the arduino to try again?

//                     isConnectedToWebSocket = false;
//                 }
//             }
//             else
//             {
//                 consoleLog("Invalid Hash " + hash);
//                 if (Connection::pairWithServer())
//                 {
//                     consoleLog("Successfully pair with server, let's wait for accept response");
//                 }
//                 else
//                 {
//                     consoleLog("Failed pair with server");

//                     // If we couldn't pair with the server we should wait and try again
//                     // TODO: We should reboot the arduino to try again?
//                 }

//                 isConnectedToWebSocket = false;
//             }
//         }
//         else
//         {
//           consoleLog("Server connection failed");
//         }
//     }
//     else
//     {
//         consoleLog("WiFi connection failed");

//         isConnectedToWiFi = false;

//         Connection::beginAP();
//     }
// }

bool Connection::connectToNetwork(String wnet, String passwd, bool retry = true)
{
    // The WiFi SSID always exists and must be valid
    if (!isValidWiFiSSID(wnet))
    {
        consoleLog("Not valid SSID: " + wnet);
        return false;
    }

    // If the WiFi password exists must be valid
    if (passwd.length() > 0 && !isValidWiFiPassword(passwd))
    {
        consoleLog("Not valid PASS: " + passwd);
        return false;
    }

    // Auto connect racing issue
    if (WiFi.status() == WL_CONNECTED && (WiFi.SSID() == wnet))
    {
        consoleLog("Already connected " + wnet);
        return true;
    }

    // If we have a valid WiFi password we'll try to connect to the network using it
    if (isValidWiFiPassword(passwd))
    {
        consoleLog("Trying to connect to SSID: " + wnet + " PASS: " + passwd);
        WiFi.begin(wnet.c_str(), passwd.c_str(), 0, NULL, true);
    }
    // If there is not a WiFi password we'll assume that the network hasn't protection (WPS)
    else
    {
        consoleLog("Trying to connect to SSID: " + wnet);
        WiFi.begin(wnet.c_str(), NULL, 0, NULL, true);
    }

    delay(5000);

    // Wait while the WiFi is trying to connect
    while (Connection::isNetworkStatus(WL_IDLE_STATUS) || Connection::isNetworkStatus(WL_NO_SHIELD))
    {
        delay(500);
    }

    // WiFi successfully connected
    if (Connection::isNetworkStatus(WL_CONNECTED))
    {
        return true;
    }

    // WiFi disconnected
    if (Connection::isNetworkStatus(WL_DISCONNECTED) && retry)
    {
        WiFi.persistent(false);
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_STA);

        delay(5000);

        consoleLog("Trying to reconnect");

        // Try to reconnect
        return Connection::connectToNetwork(wnet, passwd, false);
    }

    // There is an connection error
    consoleLog("WiFi connection error: " + String(WiFi.status()));

    return false;
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

bool Connection::connectToServer()
{
    consoleLog("Trying to connect to server: " + charToString(API_HOST) + " " + String(API_PORT));

    return wifiClient.connect(API_HOST, API_PORT);
}

#endif