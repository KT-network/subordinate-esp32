#include <Arduino.h>
#include <WiFi.h>
#include <Utils/Utils.h>
#include <Utils/DataBase.h>
#include <Utils/HttpService.h>
#include <PubSubClient.h>

#include <FastLED_NeoMatrix.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>

String devicesType = "test";
String subordinateId;

void http_callback(String json, String id, WiFiClient client, void *contex);
void wifi_ap();
bool wifi_sta(WifiInfoBase data);
void mqtt_reconnect();
void callback(char *topic, byte *payload, unsigned int length);

WiFiClass wifi;
WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);
Tool tool;
EasyTime et;
HttpServer http;
HttpResponseData httpRes;

// 配置信息的基类
MqttInfoBase mqttBase;
WifiInfoBase wifiBase;

// 每秒标志位
uint8_t tick = 61;
uint8_t tickH = 61;

// mqtt状态（mqtt服务器保活）topic
// String mqttKeepTopic;

#define pixel_w 8
#define pixel_h 8
#define DATA_PIN 6
#define NUM_LEDS (pixel_w * pixel_h)
uint8_t max_bright = 1;

CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix;
void initMatrix()
{
  matrix = new FastLED_NeoMatrix(leds, pixel_w, pixel_h, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE);
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  matrix->begin();
  matrix->setTextWrap(false);
  matrix->setBrightness(max_bright);
  matrix->clear();
  matrix->fillScreen(matrix->Color(255, 255, 255));
  matrix->show();

  // delay(1000);
  // FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // delay(1000);
  // for (int i = 0; i <= 63; i++)
  // {
  //   leds[i] = CRGB(0, 10, 0);
  //   FastLED.show();
  //   delay(10);
  // }
  // FastLED.show();
}

void setup()
{

  tool.init();
  // initMatrix();

  subordinateId = tool.get_devices_id();
  Serial.println(tool.get_general());
  // tool.del_general();
  // 为配置时，打开ap模式
  if (tool.get_general() == "")
  {
    Serial.println("wifi wei peizhi");
    int retry_times = 0;

    WiFi.begin("TP-LINK_803F", "ks123456");

    Serial.print("[WIFI]WIFI连接中");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      retry_times++;
      if (retry_times >= 50)
      {
        Serial.println();
        Serial.println("[WIFI]WIFI连接超时");
      }
    }

    Serial.println();
    Serial.print("[WIFI]WIFI连接成功,IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(",MAC: ");
    Serial.println(WiFi.macAddress());

    // wifi_ap();
    // http.setCallback(http_callback, nullptr);
    // http.start();
  }
  else
  {
    // mqttKeepTopic = "ks/subordinate/" + tool.get_general() + "/" + tool.get_devices_id() + "/state";

    Serial.println("wifi yi peizhi");

    mqttBase = tool.get_mqtt_info();

    wifiBase = tool.get_wifi_info();

    //=====================================

    int retry_times = 0;

    WiFi.begin("TP-LINK_803F", "ks123456");

    Serial.print("[WIFI]WIFI连接中");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      retry_times++;
      if (retry_times >= 50)
      {
        Serial.println();
        Serial.println("[WIFI]WIFI连接超时");
      }
    }

    Serial.println();
    Serial.print("[WIFI]WIFI连接成功,IP: ");
    Serial.print(WiFi.localIP());
    Serial.print(",MAC: ");
    Serial.println(WiFi.macAddress());

    //=====================================

    // int wifiAnewNum = 5;
    // while (wifiAnewNum > 0)
    // {

    //   if (wifi_sta(wifiBase))
    //   {
    //     break;
    //   }
    //   Serial.println(wifi.status());
    //   delay(1000);
    //   wifiAnewNum--;
    // }
    // Serial.print("wifi connect state: ");

    // Serial.println(wifi.status());

    // if (wifi.status() != WL_CONNECTED)
    // {
    //   Serial.println("wifi connect error");
    //   // 执行重新配网模式
    // }
    // Serial.println("wifi connect ok");

    //   // et.sync();
    //   IPAddress mqttIp(mqttBase.getHost1(), mqttBase.getHost2(), mqttBase.getHost3(), mqttBase.getHost4());
    //   mqtt.setServer(mqttIp, mqttBase.getPort());
    //   mqtt.setCallback(callback);
    //   mqtt.setKeepAlive(3);
    //   // mqtt.connect(tool.get_devices_id().c_str(), mqttBase.getName().c_str(), mqttBase.getPwd().c_str());
    //   // if (mqtt.connected())
    //   // {
    //   //   mqtt.subscribe("/test");
    //   // }
    //   // mqtt.subscribe("/test");
  }

  initMatrix();
}

void loop()
{
  // Serial.println("loop");

  // if (!mqtt.connected())
  // {
  //   Serial.println("mqtt-dis");

  //   mqtt_reconnect();
  // }
  // if (mqtt.connected())
  // {
  //   mqtt.loop();
  // }

  // if (tick != et.getSecond())
  // {
  //   // 每秒检查一次wifi，mqtt连接状态
  //   if (wifi.status() != WL_CONNECTED)
  //   {
  //     Serial.println("wifi-dis");

  //     // wifi 重新连接连接
  //     wifi_sta(tool.get_wifi_info());
  //   }

  //   if (!mqtt.connected())
  //   {
  //     Serial.println("mqtt-dis");

  //     mqtt_reconnect();
  //   }
  // }

  // if (mqtt.connected() && tick != et.getSecond())
  // {
  //   mqtt.loop();
  // }

  // if (et.getSecond() % 5 == 0 && tick != et.getSecond() && mqtt.connected() && wifi.status() == WL_CONNECTED)
  // {
  //   // 每5秒，发布mqtt状态（mqtt服务器保活）
  //   // mqtt.publish(mqttKeepTopic.c_str(), MqttPublishMsg::mqtt_keep_info(et.getSecond()).c_str());
  // }

  // if (et.getHour() % 2 == 0 && tickH != et.getHour())
  // {
  //   Serial.println("time-sync");
  //   // 两小时同步一次ntp时间
  //   et.sync();
  // }

  // if (tick != et.getSecond())
  // {
  //   tick = et.getSecond();
  //   tickH = et.getHour();
  // }

  // et.loop();

  // delay(500);
}

void wifi_ap()
{
  Serial.println(tool.get_devices_id());
  int s = tool.get_devices_id().length();
  const String ap_ssid = "SUBO_TEST-" + tool.get_devices_id().substring(0, 4);
  const char *ap_pwd = "123456789";
  IPAddress ip(192, 168, 5, 1);
  IPAddress gateway(192, 168, 5, 1);
  IPAddress subnet(255, 255, 255, 0);
  wifi.mode(WIFI_MODE_APSTA);
  wifi.softAP(ap_ssid.c_str(), ap_pwd);
  wifi.softAPConfig(ip, gateway, subnet);
  wifi.enableAP(true);
}

void http_callback(String json, String id, WiFiClient client, void *contex)
{

  Serial.println("start peizhi");
  StaticJsonDocument<768> jo;

  DeserializationError error = deserializeJson(jo, json);

  if (error.c_str() != "Ok")
  {
    Serial.println(error.c_str());
    return;
  }
  if (jo["type"] == "handshake")
  {
    http.response(httpRes.handshake(tool.get_devices_id(), devicesType), client);
  }
  else if (jo["type"] == "verifyDevicesInfo")
  {
    bool state = wifi_sta(tool.wifiJsonToBase(jo["data"]));
    http.response(httpRes.config_verify_wifi(state), client);
  }
  else if (jo["type"] == "setDevicesInfo")
  {
    JsonVariant data = jo["data"];
    JsonArray ips = data["ip"];
    String mqttStr = tool.jsonToString(data["mqtt"]);
    String wifiStr = tool.jsonToString(data["wifi"]);
    tool.set_mqtt_info(mqttStr);
    tool.set_wifi_info(wifiStr);
    tool.set_general(data["id"]);
    http.response(httpRes.config_set_info(true), client);
    Serial.println(httpRes.config_set_info(true));
    // http.close();
  }
  else if (jo["type"] == "restart")
  {
    Serial.println("restart");
    ESP.restart();
  }

  jo.clear();
}

// ======================================================================
bool wifi_sta(WifiInfoBase data)
{
  int overTime = 15;

  // JsonArray ips = data.getMaskCode();
  // Serial.println((uint8_t)ips[0]);
  // Serial.println((uint8_t)ips[1]);
  // Serial.println((uint8_t)ips[2]);
  // Serial.println((uint8_t)ips[3]);

  if (wifi.getMode() == WIFI_MODE_NULL)
  {
    wifi.mode(WIFI_MODE_STA);
  }
  // wifi.setAutoConnect(true);
  wifi.begin(data.getSsid().c_str(), data.getPwd().c_str());

  if (data.getDhcp() == 1)
  {
    IPAddress ip(data.getIp1(), data.getIp2(), data.getIp3(), data.getIp4());
    IPAddress gateway(data.getGat1(), data.getGat2(), data.getGat3(), data.getGat4());
    IPAddress subnet(data.getMask1(), data.getMask2(), data.getMask3(), data.getMask4());
    wifi.config(ip, gateway, subnet);
  }

  wifi.enableSTA(true);
  while (overTime > 0)
  {
    if (wifi.status() == WL_CONNECTED)
    {
      // Serial.println("wifi cg");
      return true;
      /* code */
    }
    delay(1000);
    overTime--;
  }
  // Serial.println("wifi sb");
  // wifi.enableSTA(false);
  return false;
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic); // 打印主题信息
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]); // 打印主题内容
  }
  Serial.println();

  // if ((char)payload[0] == '1') {
  //   digitalWrite(BUILTIN_LED, HIGH);   // 亮灯
  // } else {
  //   digitalWrite(BUILTIN_LED, LOW);   // 熄灯
  // }
}

void mqtt_reconnect()
{
  String topic = "ks/subordinate/" + tool.get_devices_id() + "/action";
  int overTime = 10;
  mqtt.connect(tool.get_devices_id().c_str(), mqttBase.getName().c_str(), mqttBase.getPwd().c_str());
  while (overTime > 0)
  {
    if (mqtt.connected())
    {
      mqtt.subscribe(topic.c_str());
      break;
    }
    delay(800);
    overTime--;
  }
}