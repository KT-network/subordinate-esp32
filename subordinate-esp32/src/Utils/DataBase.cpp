#include "DataBase.h"

void WifiInfoBase::setDhcp(int i)
{
    dhcp = i;
}

void WifiInfoBase::setSsid(String c)
{
    ssid = c;
}

void WifiInfoBase::setPwd(String c)
{
    pwd = c;
}

void WifiInfoBase::setIp(JsonArray c)
{
    ip = c;
}

void WifiInfoBase::setMaskCode(JsonArray c)
{
    maskCode = c;
}

void WifiInfoBase::setGateway(JsonArray c)
{
    gateway = c;
}

void WifiInfoBase::setDns(JsonArray c)
{
    dns = c;
}



int WifiInfoBase::getDhcp()
{
    return dhcp;
}

String WifiInfoBase::getSsid()
{
    return ssid;
}

String WifiInfoBase::getPwd()
{
    return pwd;
}

JsonArray WifiInfoBase::getIp()
{
    return ip;
}

JsonArray WifiInfoBase::getMaskCode()
{
    return maskCode;
}

JsonArray WifiInfoBase::getGateway()
{
    return gateway;
}

JsonArray WifiInfoBase::getDns()
{
    return dns;
}

// uint8_t WifiInfoBase::getIp1()
// {
//     return ip_1;
// }
// uint8_t WifiInfoBase::getIp2()
// {
//     return ip_2;
// }
// uint8_t WifiInfoBase::getIp3()
// {
//     return ip_3;
// }

// uint8_t WifiInfoBase::getIp4()
// {
//     return ip_4;
// }



// ==================================================

void MqttInfoBase::setHost(JsonArray s)
{
    host = s;
}

void MqttInfoBase::setPort(uint16_t s)
{
    port = s;
}

void MqttInfoBase::setName(String s)
{
    name = s;
}

void MqttInfoBase::setPwd(String s)
{
    pwd = s;
}

JsonArray MqttInfoBase::getHost()
{
    return host;
}

uint16_t MqttInfoBase::getPort()
{
    return port;
}
String MqttInfoBase::getName()
{
    return name;
}
String MqttInfoBase::getPwd()
{
    return pwd;
}
