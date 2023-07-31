#include "HttpService.h"

void HttpServer::start()
{
    Serial.println("[Config]开启Http Service");
    server = WiFiServer(80);
    server.begin();
    state = true;
    __listener();
}

void HttpServer::__listener()
{
    while (state)
    {
        // WiFiClient client = server.accept();
        WiFiClient client = server.available();
        if (client.available())
        {

            String receive = client.readString();
            Serial.println(receive);

            String jsonStr = receive.substring(receive.indexOf("\n{") + 1, receive.length());
            String generalIdStr = receive.substring(receive.indexOf("GeneralId: "), receive.indexOf("Content-Type"));
            // Serial.println(jsonStr);
            _callback(jsonStr, generalIdStr, client, nullptr);
        }
        // client.stop();
        /* code */
        delay(50);
    }
}

void HttpServer::response(String body, WiFiClient client)
{
    client.println("HTTP/1.1 200 OK");
    // client.println("Content-type:text/html");
    client.println();

    client.print(body);
    client.println();

    client.stop();
    Serial.println("response: cg");
}

void HttpServer::setCallback(CallbackFun callback, void *contex)
{    
    Serial.println("[Config]开始监听Http Service 消息");
    _callback = callback;
}

void HttpServer::close()
{
    state = false;
    server.close();
}