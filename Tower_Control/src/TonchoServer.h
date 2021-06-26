#ifndef Toncho_Server_h
#define Toncho_Server_h

#include <ESPAsyncWebServer.h>

class TonchoServer
{
    public:
        TonchoServer(int port, String wsPath, bool healthCheck);
        static void handleNotFound(AsyncWebServerRequest *request);
        void get(const char* uri, ArRequestHandlerFunction handler);
        void page(const char* uri, const char* source);
        void page(const char* uri, const char* source, String mimeType);
        void page(const char* uri, const char* source, ArRequestHandlerFunction handler);
        void page(const char* uri, const char* source, String mimeType, ArRequestHandlerFunction handler);
        void socket(AwsEventHandler handler);
        void socketCleanup();
        void begin();
    private:
        int _port;
        String _wsPath;
        bool _healthCheck;
        AsyncWebServer *_server;
        AsyncWebSocket *_ws;
        void on(const char* uri, WebRequestMethodComposite method, ArRequestHandlerFunction handler);
        void onSocketEvent(AwsEventHandler handler);
};

#endif
