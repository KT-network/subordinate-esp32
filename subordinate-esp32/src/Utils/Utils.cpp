#include "Utils.h"

void Tool::init()
{
  // devicesGeneralNvs.begin("general");
  // wifiNvs.begin("wifi");
  // mqttNvs.begin("mqtt");
  nvs.begin("config");
  Serial.begin(115200);
}
// 获取设备id
String Tool::get_devices_id()
{
  return String(ESP.getEfuseMac(), HEX);
}

// app id
String Tool::get_general()
{
  if (nvs.isKey("general"))
  {
    return nvs.getString("general");
  }
  else
  {
    return "";
  }
}

bool Tool::set_general(String s)
{
  size_t state = nvs.putString("general", s);
  return state <= 0 ? false : true;
}

bool Tool::del_general()
{
  if (nvs.isKey("general"))
  {
    return nvs.remove("general");
  }
  return false;
}

// wifi信息
WifiInfoBase Tool::get_wifi_info()
{
  if (nvs.isKey("wifi"))
  {
    String s = nvs.getString("wifi");
    // Serial.println(s);
    // WifiInfoBase base = wifiStrToBase(s);
    // Serial.println((int)base.getIp()[0]);

    // char a[20];
    // sprintf((char *)a, "%x", base.getIp());
    // Serial.println(a);
    return wifiStrToBase(s);
  }
}

bool Tool::set_wifi_info(String s)
{
  size_t state = nvs.putString("wifi", s);
  return state <= 0 ? false : true;
}

bool Tool::del_wifi_info()
{
  if (nvs.isKey("wifi"))
  {
    return nvs.remove("wifi");
  }
  return false;
}
// mqtt信息
MqttInfoBase Tool::get_mqtt_info()
{
  if (nvs.isKey("mqtt"))
  {
    return mqttStrToBase(nvs.getString("mqtt"));
  }
}

bool Tool::set_mqtt_info(String s)
{
  size_t state = nvs.putString("mqtt", s);
  return state <= 0 ? false : true;
}

bool Tool::del_mqtt_info()
{
  if (nvs.isKey("mqtt"))
  {
    return nvs.remove("mqtt");
  }
  return false;
}

uint32_t Tool::get_freeEntries()
{
  return nvs.freeEntries();
}

WifiInfoBase Tool::wifiStrToBase(String json)
{
  WifiInfoBase base;
  StaticJsonDocument<520> wifi_jo;
  DeserializationError error = deserializeJson(wifi_jo, json);
  if (error.c_str() != "Ok")
  {
    Serial.println(error.c_str());
    return base;
  }
  base.setDhcp(wifi_jo["dhcp"]);
  base.setSsid(wifi_jo["ssid"]);
  base.setPwd(wifi_jo["pwd"]);
  // base.setIp(wifi_jo["ip"]);
  // base.setMaskCode(wifi_jo["maskCode"]);
  // base.setGateway(wifi_jo["gateway"]);
  // base.setDns(wifi_jo["dns"]);

  // Serial.println((int)base.getIp()[0]);
  // Serial.println(base.getIp().size());
  // wifi_jo.clear();

  // char a[20];
  // sprintf((char *)a, "%x", base.getIp());
  // Serial.println(a);

  base.setIp(wifi_jo["ip"][0],
             wifi_jo["ip"][1],
             wifi_jo["ip"][2],
             wifi_jo["ip"][3]);
  base.setMaskCode(wifi_jo["maskCode"][0],
                   wifi_jo["maskCode"][1],
                   wifi_jo["maskCode"][2],
                   wifi_jo["maskCode"][3]);

  base.setGateway(wifi_jo["gateway"][0],
                  wifi_jo["gateway"][1],
                  wifi_jo["gateway"][2],
                  wifi_jo["gateway"][3]);
  base.setDns(wifi_jo["dns"][0],
              wifi_jo["dns"][1],
              wifi_jo["dns"][2],
              wifi_jo["dns"][3]);
  return base;
}

WifiInfoBase Tool::wifiJsonToBase(JsonVariant wifi_jo)
{
  WifiInfoBase base;
  base.setDhcp(wifi_jo["dhcp"]);
  base.setSsid(wifi_jo["ssid"]);
  base.setPwd(wifi_jo["pwd"]);
  base.setIp(wifi_jo["ip"]);
  base.setMaskCode(wifi_jo["maskCode"]);
  base.setGateway(wifi_jo["gateway"]);
  base.setDns(wifi_jo["dns"]);

  base.setIp(wifi_jo["ip"][0],
             wifi_jo["ip"][1],
             wifi_jo["ip"][2],
             wifi_jo["ip"][3]);
  base.setMaskCode(wifi_jo["maskCode"][0],
                   wifi_jo["maskCode"][1],
                   wifi_jo["maskCode"][2],
                   wifi_jo["maskCode"][3]);

  base.setGateway(wifi_jo["gateway"][0],
                  wifi_jo["gateway"][1],
                  wifi_jo["gateway"][2],
                  wifi_jo["gateway"][3]);
  base.setDns(wifi_jo["dns"][0],
              wifi_jo["dns"][1],
              wifi_jo["dns"][2],
              wifi_jo["dns"][3]);

  return base;
}

String Tool::wifiBaseToStr(JsonVariant jo)
{
  String json;
  StaticJsonDocument<450> res;
  res["dhcp"] = jo["dhcp"];
  res["ssid"] = jo["ssid"];
  res["pwd"] = jo["pwd"];
  JsonArray ip = res.createNestedArray("ip");
  ip[0] = jo["ip"][0];
  ip[1] = jo["ip"][1];
  ip[2] = jo["ip"][2];
  ip[3] = jo["ip"][3];
  JsonArray mask = res.createNestedArray("maskCode");
  mask[0] = jo["maskCode"][0];
  mask[1] = jo["maskCode"][1];
  mask[2] = jo["maskCode"][2];
  mask[3] = jo["maskCode"][3];
  JsonArray gateway = res.createNestedArray("gateway");
  gateway[0] = jo["gateway"][0];
  gateway[1] = jo["gateway"][1];
  gateway[2] = jo["gateway"][2];
  gateway[3] = jo["gateway"][3];
  JsonArray dns = res.createNestedArray("dns");
  dns[0] = jo["dns"][0];
  dns[1] = jo["dns"][1];
  dns[2] = jo["dns"][2];
  dns[3] = jo["dns"][3];

  serializeJson(res, json);

  return json;
}

MqttInfoBase Tool::mqttStrToBase(String json)
{
  MqttInfoBase base;
  StaticJsonDocument<200> jo;
  DeserializationError error = deserializeJson(jo, json);
  if (error.c_str() != "Ok")
  {
    Serial.println(error.c_str());
    return base;
  }
  base.setHost(jo["host"]);
  base.setPort(jo["port"]);
  base.setName(jo["user"]);
  base.setPwd(jo["pwd"]);

  base.setHost(jo["host"][0], jo["host"][1], jo["host"][2], jo["host"][3]);

  return base;
}

MqttInfoBase Tool::mqttJsonToBase(JsonVariant jo)
{
  MqttInfoBase base;
  base.setHost(jo["host"]);
  base.setPort(jo["port"]);
  base.setName(jo["user"]);
  base.setPwd(jo["pwd"]);

  base.setHost(jo["host"][0], jo["host"][1], jo["host"][2], jo["host"][3]);

  return base;
}

String Tool::jsonToString(JsonVariant jo)
{
  String s;
  serializeJson(jo, s);
  return s;
}

EasyTime::EasyTime()
{
  gmtOffset_sec = 8 * 3600;
  daylightOffset_sec = 0;
  server1 = NTP1;
  server2 = NTP2;
  server3 = NTP3;
}

EasyTime::EasyTime(long gmtOffset, int daylightOffset, const char *ser1, const char *ser2, const char *ser3)
{
  gmtOffset_sec = gmtOffset;
  daylightOffset_sec = daylightOffset;
  server1 = ser1;
  server2 = ser2;
  server3 = ser3;
}

void EasyTime::sync()
{
  int timeAnewNum = 3;
  configTime(gmtOffset_sec, daylightOffset_sec, server1, server2, server3);

  if (!getLocalTime(&timeinfo, 5000))
  {
    while (timeAnewNum > 0)
    {
      Serial.println("sync");
      configTime(gmtOffset_sec, daylightOffset_sec, server1, server2, server3);
      if (getLocalTime(&timeinfo, 3000))
      {
        succeed = true;
        return;
      }
      delay(1000);
      timeAnewNum--;
    }

    succeed = false;
    return;
  }
  succeed = true;
}

void EasyTime::loop()
{

  if (!succeed || hourTick != getHour())
  {
    configTime(gmtOffset_sec, daylightOffset_sec, server1, server2, server3);
    if (getLocalTime(&timeinfo, 50))
    {
      succeed = true;
      hourTick = getHour();
    }else{
      succeed = false;
    }
  }
  else
  {
    getLocalTime(&timeinfo, 50);
  }
}