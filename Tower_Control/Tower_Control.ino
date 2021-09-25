#include "Config.h"

// HTML LOCAL CONTROL PAGE
#include "pages/localControl.h"

#include "src/Actuator.h"
#include "src/Connection.h"
#include "src/Panel.h"
#include "src/Serial.h"
#include "src/Timer.h"
#include "src/TonchoServer.h"
#include "src/Utils.h"

Actuator actuator(ELEVATION_RELAY_UP, ELEVATION_RELAY_DOWN, INCLINATION_RELAY_UP, INCLINATION_RELAY_DOWN, RELAY_NO);

Connection connection;

Timer timer(TIMER_CONTROL_RELAY, MILLISECONDS, RELAY_NO);

TonchoServer server(SERVER_PORT, WEB_SOCKET_PATH, true);

Panel panel;

// Helpers

void handleAction(AsyncWebServerRequest *request, int action)
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
    if (direction == 1)
    {
        panel.navigate(PAGE_ELEVATE_UP);
    } else {
        panel.navigate(PAGE_ELEVATE_DOWN);
    }
  }
  else if (action == ACTION_INCLINATION)
  {
    status = actuator.inclinate(direction);
    if (direction == 1)
    {
        panel.navigate(PAGE_INCLINATE_UP);
    } else {
        panel.navigate(PAGE_INCLINATE_DOWN);
    }
  }

  if (!status)
  {
    // Bad Request
    return request->send(400, "text/plain", "Invalid direction for " + actionName);
  }

  return request->send(200, "text/plain", actionName);
}

// HTTP HANDLERS

void handleLocalControl(AsyncWebServerRequest *request)
{
  String page = LOCAL_CONTROL_PAGE;
  request->send(200, "text/html", page);
}

void handleElevation(AsyncWebServerRequest *request)
{
  handleAction(request, ACTION_ELEVATION);
  timer.start();
}

void handleInclination(AsyncWebServerRequest *request)
{
  handleAction(request, ACTION_INCLINATION);
}

void handleStop(AsyncWebServerRequest *request)
{
  actuator.stop();
  timer.stop();
  panel.navigate(PAGE_STOP);

  return request->send(200, "text/plain", "stop");
}

void handleConfig(AsyncWebServerRequest *request)
{
  return request->send(200, "text/plain");
}

void setup(void)
{

  setupSerial(SERIAL_PORT);

  actuator.begin();

  timer.begin();

  connection.begin();

  // Add Web Server Routes
  server.get("/", handleLocalControl);
  server.get("/elevation", handleElevation);
  server.get("/inclination", handleInclination);
  server.get("/stop", handleStop);
  server.get("/config", handleConfig);

  server.page("/local_control", LOCAL_CONTROL_PAGE);

  // Start Web Server
  server.begin();

  consoleLog("\n\n\tTower Control Webserver started\n\n");
  
  panel.begin();
}

// LOOP
void loop(void)
{
  panel.loop();
  timer.loop();
}
