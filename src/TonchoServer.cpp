#include <ESPAsyncWebServer.h>

#include "TonchoServer.h"

TonchoServer::TonchoServer(int port, String wsPath, bool healthCheck)
{
    _port = port;
    _wsPath = wsPath;

    _healthCheck = healthCheck;
    
    _server = new AsyncWebServer(_port);
    _ws = new AsyncWebSocket(_wsPath);
}

void TonchoServer::handleNotFound(AsyncWebServerRequest *request)
{
    String message = "Not Found \t URL: \t" + request->url();

    return request->send(404, "text/plain", message);
}

void TonchoServer::on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction handler)
{
    _server->on(uri, method, handler);
}

void TonchoServer::onSocketEvent(AwsEventHandler handler)
{
    _ws->onEvent(handler);
}

void TonchoServer::get(const char* uri, ArRequestHandlerFunction handler)
{
    TonchoServer::on(uri, HTTP_GET, handler);
}

void TonchoServer::page(const char* uri, const char* source)
{
    TonchoServer::get(uri, [ = ](AsyncWebServerRequest *request) {
      request->send(200, "text/html", source);
    });
}

void TonchoServer::page(const char* uri, const char* source, String mimeType)
{
    TonchoServer::get(uri, [ = ](AsyncWebServerRequest *request) {
      request->send(200, mimeType, source);
    });
}

void TonchoServer::page(const char* uri, const char* source, ArRequestHandlerFunction handler)
{
    TonchoServer::get(uri, [ = ](AsyncWebServerRequest *request) {
      request->send(200, "text/html", source);
      handler(request);
    });
}

void TonchoServer::page(const char* uri, const char* source, String mimeType, ArRequestHandlerFunction handler)
{
    TonchoServer::get(uri, [ = ](AsyncWebServerRequest *request) {
      request->send(200, mimeType, source);
      handler(request);
    });
}

void TonchoServer::socket(AwsEventHandler handler)
{
    TonchoServer::onSocketEvent(handler);
}

void TonchoServer::socketCleanup()
{
    _ws->cleanupClients();
}

void TonchoServer::begin()
{
    // Route definitions
    _server->onNotFound(TonchoServer::handleNotFound);
    if (_healthCheck)
    {
        TonchoServer::get("/health", [ = ](AsyncWebServerRequest *request) {
            request->send(200);
        });
    }

    _server->addHandler(_ws);

    // Start Web Server
    _server->begin();
}
