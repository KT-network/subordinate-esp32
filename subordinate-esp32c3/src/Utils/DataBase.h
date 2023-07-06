#ifndef __DATABASE_H
#define __DATABASE_H

#include "WString.h"
#include "ArduinoJson.h"
class WifiInfoBase
{
private:
    int dhcp;
    String ssid;
    String pwd;
    JsonArray ip;
    JsonArray maskCode;
    JsonArray gateway;
    JsonArray dns;

    uint8_t ip_1;
    uint8_t ip_2;
    uint8_t ip_3;
    uint8_t ip_4;

    uint8_t mask_1;
    uint8_t mask_2;
    uint8_t mask_3;
    uint8_t mask_4;

    uint8_t gat_1;
    uint8_t gat_2;
    uint8_t gat_3;
    uint8_t gat_4;

    uint8_t dns_1;
    uint8_t dns_2;
    uint8_t dns_3;
    uint8_t dns_4;

public:
    int getDhcp();
    String getSsid();
    String getPwd();
    JsonArray getIp();
    JsonArray getMaskCode();
    JsonArray getGateway();
    JsonArray getDns();

    uint8_t getIp1() { return ip_1; };
    uint8_t getIp2() { return ip_2; };
    uint8_t getIp3() { return ip_3; };
    uint8_t getIp4() { return ip_4; };

    uint8_t getMask1() { return mask_1; };
    uint8_t getMask2() { return mask_2; };
    uint8_t getMask3() { return mask_3; };
    uint8_t getMask4() { return mask_4; };

    uint8_t getGat1() { return gat_1; };
    uint8_t getGat2() { return gat_2; };
    uint8_t getGat3() { return gat_3; };
    uint8_t getGat4() { return gat_4; };

    uint8_t getDns1() { return dns_1; };
    uint8_t getDns2() { return dns_2; };
    uint8_t getDns3() { return dns_3; };
    uint8_t getDns4() { return dns_4; };

    void setDhcp(int i);
    void setSsid(String c);
    void setPwd(String c);
    void setIp(JsonArray c);
    void setMaskCode(JsonArray c);
    void setGateway(JsonArray c);
    void setDns(JsonArray c);

    void setIp(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3)
    {
        ip_1 = i;
        ip_2 = i1;
        ip_3 = i2;
        ip_4 = i3;
    };
    // void setIp2(uint8_t i);
    // void setIp3(uint8_t i);
    // void setIp4(uint8_t i);

    void setMaskCode(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3)
    {
        mask_1 = i;
        mask_2 = i1;
        mask_3 = i2;
        mask_4 = i3;
    };
    // void setMask2(uint8_t i) { mask_2 = i; };
    // void setMask3(uint8_t i) { mask_3 = i; };
    // void setMask4(uint8_t i) { mask_4 = i; };

    void setGateway(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3)
    {
        gat_1 = i;
        gat_2 = i1;
        gat_3 = i2;
        gat_4 = i3;
    };
    // void setGat2(uint8_t i){gat_1 = i;};
    // void setGat3(uint8_t i){gat_1 = i;};
    // void setGat4(uint8_t i){gat_1 = i;};

    void setDns(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3)
    {
        dns_1 = i;
        dns_2 = i1;
        dns_3 = i2;
        dns_4 = i3;
    };
    // void setDns2(uint8_t i){};
    // void setDns3(uint8_t i){};
    // void setDns4(uint8_t i){};
};

class MqttInfoBase
{
private:
    JsonArray host;
    uint16_t port;
    String name;
    String pwd;

    uint8_t host_1;
    uint8_t host_2;
    uint8_t host_3;
    uint8_t host_4;

public:
    JsonArray getHost();
    uint16_t getPort();
    String getName();
    String getPwd();

    uint8_t getHost1() { return host_1; };
    uint8_t getHost2() { return host_2; };
    uint8_t getHost3() { return host_3; };
    uint8_t getHost4() { return host_4; };

    void setHost(JsonArray s);
    void setPort(uint16_t s);
    void setName(String s);
    void setPwd(String s);

    void setHost(uint8_t i, uint8_t i1, uint8_t i2, uint8_t i3)
    {
        host_1 = i;
        host_2 = i1;
        host_3 = i2;
        host_4 = i3;
    };
};

#endif