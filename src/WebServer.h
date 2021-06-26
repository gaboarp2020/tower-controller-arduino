#ifndef Web_Server_h
#define Web_Server_h

#include <ESPAsyncWebServer.h>

class WebServer
{
    public:
        WebServer(int port, String wsPath, bool healthCheck);
        static void handleNotFound(AsyncWebServerRequest *request);
        void get(const char* uri, ArRequestHandlerFunction handler);
        void page(const char* uri, const char* source);
        void page(const char* uri, const char* source, String mimeType);
        void page(const char* uri, const char* source, ArRequestHandlerFunction handler);
        void page(const char* uri, const char* source, String mimeType, ArRequestHandlerFunction handler);
        void begin();
    private:
        int _port;
        bool _healthCheck;
        AsyncWebServer *_server;
        void on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction handler);
};

#endif