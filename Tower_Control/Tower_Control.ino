#include "Config.h"

// HTML LOCAL CONTROL PAGE
#include "pages/localControl.h"

#include "src/Actuator.h"
#include "src/Connection.h"
#include "src/Serial.h"
#include "src/TonchoServer.h"
#include "src/Utils.h"

Actuator actuator(ELEVATION_RELAY_UP, ELEVATION_RELAY_DOWN, INCLINATION_RELAY_UP, INCLINATION_RELAY_DOWN);

Connection connection;

TonchoServer server(SERVER_PORT, WEB_SOCKET_PATH, true);

// Helpers

void handleAction(AsyncWebServerRequest *request, int action);
{
  String actionName = action == ACTION_ELEVATION ? "Elevation" : "Inclination";

  if (!request->hasArg("direction"))
  {
    // Bad Request
    return request->send(400, "text/plain", "Invalid direction for " + actionName);
  }

  // Arguments
  int direction = request->arg("direction").toInt();

  bool status = false;

  if (action == ACTION_ELEVATION)
  {
    status = actuator.elevate(direction);
  }
  else if (action == ACTION_INCLINATION)
  {
    status = actuator.inclination(direction);
  }

  if (!status)
  {
    // Bad Request
    return request->send(400, "text/plain", "Invalid direction for " + actionName);
  }

  return request->send(200);
}

// HTTP HANDLERS

void handleElevation(AsyncWebServerRequest *request)
{
  handleAction(request, ACTION_ELEVATION);
}

void handleInclination(AsyncWebServerRequest *request)
{
  handleAction(request, ACTION_INCLINATION);
}

void handleStop(AsyncWebServerRequest *request)
{
  actuator.stop();

  return request->send(200);
}

void handleConfig(AsyncWebServerRequest *request)
{
  return request->send(200, "text/plain");
}

void setup(void)
{

  setupSerial(SERIAL_PORT);

  actuator.begin();

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
