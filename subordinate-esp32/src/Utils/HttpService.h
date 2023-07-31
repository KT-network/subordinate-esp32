#ifndef __HTTPSERVICE_H
#define __HTTPSERVICE_H

#include <WiFi.h>
#include <WiFiClient.h>
class HttpServer
{
private:
    typedef void (*CallbackFun)(String json, String id,WiFiClient client, void *contex);

    WiFiServer server;
    bool state = false;
    void __listener();
    CallbackFun _callback;
    /* data */
public:
    void start();
    void close();
    void response(String body,WiFiClient client);
    void setCallback(CallbackFun callback, void *contex);
};



#endif