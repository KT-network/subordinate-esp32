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
void mqttMsgManage(byte payload[], int length);

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
#define DATA_PIN 5
#define NUM_LEDS (pixel_w * pixel_h)
uint8_t max_bright = 20;

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
}

void setup()
{

  tool.init();

  initMatrix();

  subordinateId = tool.get_devices_id();
  Serial.println("[Config]设备id:" + subordinateId);

  // tool.del_general();
  // 为配置时，打开ap模式
  if (tool.get_general() == "")
  {
    Serial.println();
    Serial.println("[Config]设备未配置");
    wifi_ap();
    http.setCallback(http_callback, nullptr);
    http.start();
  }
  else
  {
    Serial.println();
    Serial.println("[Config]设备已配置");
    Serial.println("[Config]绑定的Userid:" + tool.get_general());

    mqttBase = tool.get_mqtt_info();

    wifiBase = tool.get_wifi_info();
    Serial.print("[Config]WiFi名称:");
    Serial.print(wifiBase.getSsid());
    Serial.println();

    Serial.print("[Config]WiFi密码:");
    Serial.print(wifiBase.getPwd());
    Serial.println();

    Serial.print("[Config]Mqtt用户:");
    Serial.print(mqttBase.getName());
    Serial.println();

    Serial.print("[Config]Mqtt密码:");
    Serial.print(mqttBase.getPwd());

    //=====================================

    // int retry_times = 0;

    // WiFi.begin("TP-LINK_803F", "ks123456");

    // Serial.print("[WIFI]WIFI连接中");
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //   delay(500);
    //   Serial.print(".");
    //   retry_times++;
    //   if (retry_times >= 50)
    //   {
    //     Serial.println();
    //     Serial.println("[WIFI]WIFI连接超时");
    //   }
    // }

    // Serial.println();
    // Serial.print("[WIFI]WIFI连接成功,IP: ");
    // Serial.print(WiFi.localIP());
    // Serial.print(",MAC: ");
    // Serial.println(WiFi.macAddress());

    //=====================================

    int wifiAnewNum = 5;
    while (wifiAnewNum > 0)
    {

      if (wifi_sta(wifiBase))
      {
        break;
      }
      Serial.println(wifi.status());
      delay(1000);
      wifiAnewNum--;
    }

    if (wifi.status() != WL_CONNECTED)
    {
      Serial.println("wifi connect error");
      // 执行重新配网模式
    }

    // et.sync();
    IPAddress mqttIp(mqttBase.getHost1(), mqttBase.getHost2(), mqttBase.getHost3(), mqttBase.getHost4());
    mqtt.setServer(mqttIp, mqttBase.getPort());
    mqtt.setCallback(callback);
    mqtt.setKeepAlive(3);
  }
}

void loop()
{

  if (!mqtt.connected())
  {
    Serial.println();
    Serial.println("[MQTT]MQTT断开连接");
    mqtt_reconnect();
  }
  if (mqtt.connected())
  {
    mqtt.loop();
  }

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

/*配置模式时，开启设备的热点，ssid -> SUBO_设备类型-设备id的前4位，密码 -> 123456789*/
void wifi_ap()
{
  Serial.println("[Config]开启热点模式");
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

/*配置模式时，开启http服务器，监听http回调信息*/
void http_callback(String json, String id, WiFiClient client, void *contex)
{
  StaticJsonDocument<768> jo;
  DeserializationError error = deserializeJson(jo, json);
  if (error.c_str() != "Ok")
  {
    Serial.println(error.c_str());
    return;
  }
  // http 握手，并返回设备id
  if (jo["type"] == "handshake")
  {
    http.response(httpRes.handshake(tool.get_devices_id(), devicesType), client);
  }
  // 验证配置信息（测试wifi连接）
  else if (jo["type"] == "verifyDevicesInfo")
  {
    bool state = wifi_sta(tool.wifiJsonToBase(jo["data"]));
    http.response(httpRes.config_verify_wifi(state), client);
  }
  // 保存配置信息
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
  // 重启设备
  else if (jo["type"] == "restart")
  {
    Serial.println("restart");
    ESP.restart();
  }

  jo.clear();
}

/*开启设备的wifi，并从保存的配置信息里连接*/
bool wifi_sta(WifiInfoBase data)
{
  Serial.println();
  Serial.print("[WIFI]WiFi连接中");

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
  // dhcp为1的时候设置静态ip
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
      Serial.println();
      Serial.println("[WIFI]Wifi连接成功");
      return true;
      /* code */
    }
    Serial.print(".");
    delay(1000);
    overTime--;
  }
  Serial.println("[WIFI]Wifi连接超时");
  // wifi.enableSTA(false);
  return false;
}

/*mqtt 监听回调*/
void callback(char *topic, byte *payload, unsigned int length)
{
  mqttMsgManage(payload, length);
}

/*mqtt连接*/
void mqtt_reconnect()
{
  String topic = "ks/server/subordinate/" + tool.get_devices_id() + "/action";
  int overTime = 10;
  mqtt.connect(tool.get_devices_id().c_str(), mqttBase.getName().c_str(), mqttBase.getPwd().c_str());
  while (overTime > 0)
  {
    Serial.println("[MQTT]MQTT开始连接");
    if (mqtt.connected())
    {
      mqtt.subscribe(topic.c_str());
      Serial.println("[MQTT]MQTT连接成功");
      break;
    }
    delay(800);
    overTime--;
  }
}

/*mqtt 接收到的消息处理*/
void mqttMsgManage(byte payload[], int length)
{
  switch (payload[0])
  {
  case 1:
  {
    uint8_t restart = payload[1];
    char info[length - 1];
    for (size_t i = 1; i < length; i++)
    {
      info[i - 2] = payload[i];
    }
    info[length - 1] = '\0';
    Serial.println(info);
    tool.set_wifi_info(info);
    if (restart == 1)
    {
      ESP.restart();
    }

    break;
  }
  case 2:
  {
    pinMode(payload[1], OUTPUT);
    digitalWrite(payload[1], payload[2]);
    break;
  }
  case 3:
  {
    matrix->clear();
    unsigned short color = (payload[1] << 8) + payload[2];
    matrix->fillScreen(color);
    matrix->show();
    break;
  }
  case 4:
  {
    unsigned short color = (payload[3] << 8) + payload[4];
    matrix->drawPixel(payload[1], payload[2], color);
    matrix->show();
    break;
  }
  case 5:
  {
    uint16_t x_coordinate = 0;
    uint16_t y_coordinate = 0;
    int16_t width = payload[3];
    int16_t height = payload[4];
    unsigned short colorData[width * height];
    for (int i = 0; i < width * height * 2; i++)
    {
      colorData[i / 2] = (payload[i + 5] << 8) + payload[i + 1 + 5];
      i++;
    }

    for (int16_t j = 0; j < height; j++, y_coordinate++)
    {
      for (int16_t i = 0; i < width; i++)
      {
        matrix->drawPixel(x_coordinate + i, y_coordinate, (uint16_t)colorData[j * width + i]);
      }
    }

    matrix->show();
    break;
  }
  }
}