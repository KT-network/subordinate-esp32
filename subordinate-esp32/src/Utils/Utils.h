#ifndef __UTILS_H
#define __UTILS_H

// #include <Arduino.h>
#include "Esp.h"
#include "Preferences.h"
#include "DataBase.h"
#include "ArduinoJson.h"

#define NTP1 "cn.ntp.org.cn"
#define NTP2 "time1.cloud.tencent.com"
#define NTP3 "ntp3.aliyun.com"

class Tool
{
private:
    // Preferences devicesGeneralNvs;
    // Preferences wifiNvs;
    // Preferences mqttNvs;
    Preferences nvs;
    StaticJsonDocument<250> mqtt_jo;

public:
    void init();
    String get_devices_id();
    String get_general();
    bool set_general(String s);
    bool del_general();

    WifiInfoBase get_wifi_info();
    bool set_wifi_info(String s);
    bool del_wifi_info();

    MqttInfoBase get_mqtt_info();
    bool set_mqtt_info(String s);
    bool del_mqtt_info();
    uint32_t get_freeEntries();

    WifiInfoBase wifiStrToBase(String json);
    WifiInfoBase wifiJsonToBase(JsonVariant jo);
    String wifiBaseToStr(JsonVariant jo);

    MqttInfoBase mqttStrToBase(String json);
    MqttInfoBase mqttJsonToBase(JsonVariant jo);

    String jsonToString(JsonVariant jo);
};

class HttpResponseData
{
private:
    // StaticJsonDocument<250> res;

public:
    String handshake(String id, String devicesType)
    {
        String json;
        StaticJsonDocument<500> res;
        JsonObject data = res.createNestedObject("data");
        res["type"] = "handshake";

        data["handshake"] = "hello general";
        data["id"] = id;
        data["devicesType"] = devicesType;

        serializeJson(res, json);
        return json;
    };

    String config_verify_wifi(bool state)
    {
        String json;
        StaticJsonDocument<250> res;
        JsonObject data = res.createNestedObject("data");
        res["type"] = "verifyDevicesInfo";
        data["state"] = state;

        serializeJson(res, json);
        return json;
    };

    String config_set_info(bool state)
    {
        String json;
        StaticJsonDocument<250> res;
        JsonObject data = res.createNestedObject("data");
        res["type"] = "setDevicesInfo";
        data["state"] = state;
        data["msg"] = "config succeed!";

        serializeJson(res, json);
        return json;
    }
};

class MqttPublishMsg
{
public:
    static String mqtt_keep_info(int second)
    {
        String json;
        StaticJsonDocument<250> res;
        res["action"] = "maintain";
        // res["subordinateId"] =
        res["time"] = second;
        serializeJson(res, json);
        return json;
    }
};

class EasyTime
{
private:
    long gmtOffset_sec;
    int daylightOffset_sec;
    const char *server1;
    const char *server2;
    const char *server3;
    // ntp成功
    bool succeed;
    // 每小时更新一次ntp标志位
    uint8_t hourTick = 25;

public:
    struct tm timeinfo;
    EasyTime();
    EasyTime(long gmtOffset_sec, int daylightOffset_sec, const char *server1, const char *server2, const char *server3);
    void sync();
    void loop();
    uint8_t getSecond()
    {
        return timeinfo.tm_sec;
    };

    uint8_t getHour()
    {
        return timeinfo.tm_hour;
    };

    uint8_t getMin()
    {
        return timeinfo.tm_min;
    };
};

#endif