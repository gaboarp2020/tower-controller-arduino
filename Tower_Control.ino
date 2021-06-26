#define DEBUG

// HTML LOCAL CONTROL PAGE
#include "pages/localControl.h"


#include "src/Connection.h"
#include "src/Serial.h"
#include "src/Utils.h"
#include "src/WebServer.h"

// Elevetion Actions
#define ELEVATION_RELAY_UP   14 // Elevation UP -------- GPIO14
#define ELEVATION_RELAY_DOWN 12 // Elevation DOWN ------ GPIO12

// Inclination Actions
#define INCLINATION_RELAY_UP   13 // Inclination UP ------ GPIO13
#define INCLINATION_RELAY_DOWN 15 // Inclination DOWN ---- GPIO15

#define INTERVAL 1000 // interval at which to update ws (ms)

#define SERIAL_PORT 115200

#define UP_ACTION 1
#define DOWN_ACTION 2

#define ELEVATION_ACTUATOR 1
#define INCLINATION_ACTUATOR 2

#define SERIAL_PORT 115200



Connection connection;

WebServer server(80, "/ws", true);

// HELPERS

void actuatorActions(int actuator, int action )
{

  consoleLog("Doing stuff...");
}

void stopAll()
{
  consoleLog("STOP!");
}

// HTTP HANDLERS

void handleElevation(AsyncWebServerRequest *request)
{
  stopAll();

  // TODO: Elevation logic

  actuatorActions(ELEVATION_ACTUATOR, UP_ACTION);

  return request->send(200);
}

void handleInclination(AsyncWebServerRequest *request)
{
  stopAll();

  // TODO: Inclination logic
  actuatorActions(INCLINATION_ACTUATOR, UP_ACTION);

  return request->send(200);
}

void handleStop(AsyncWebServerRequest *request)
{
  stopAll();

  return request->send(200);
}

void handleConfig(AsyncWebServerRequest *request)
{
  return request->send(200, "text/plain");
}

void setup(void)
{

  setupSerial(SERIAL_PORT);

  connection.begin();

  // Add Web Server Routes
  server.get("/elevation", handleElevation);
  server.get("/inclination", handleInclination);
  server.get("/stop", handleStop);
  server.get("/config", handleConfig);

  server.page("/local_control", LOCAL_CONTROL_PAGE);

  // Start Web Server
  server.begin();

  consoleLog("Tower Control Webserver started");
}

// LOOP
void loop(void)
{
}
