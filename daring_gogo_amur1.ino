#include <Arduino.h>
#include <ESP8266WiFi.h>
const char *ssid_STA = "nikelborm";
const char *password_STA = "12345678";
const char *ssid_AP = "ESP Web-server";
const char *password_AP = "12345678";

void initWiFi()
{
  byte tries = 11;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid_STA, password_STA);
  Serial.print("Connecting to WiFi");
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    Serial.print("");
    Serial.print("WiFi up AP");
    IPAddress apIP(192, 168, 4, 1);
    IPAddress staticGateway(192, 168, 4, 1);
    IPAddress staticSubnet(255, 255, 255, 0);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, staticGateway, staticSubnet);
    WiFi.softAP(ssid_AP, password_AP);
  }
  else
  {
    Serial.println("WiFi connecting");
    Serial.println("IP addres:");
    Serial.println(WiFi.localIP());
  }
}

void setup()
{
  Serial.begin(74880);
  while (!Serial) {}

  Serial.println(
    (String)analogRead(A0)
    + " "
    + (String)(digitalRead(123)*200)
  );

  // Ждём 20 10^6 микросекунд (20 секунд во сне)
  // Необходимо сохранить всё во флэш, например последние датчики, потому что оперативная память полностью сбросится

  ESP.deepSleep(20e6);
  // нужно учитывать что ESP.deepSleep это фактически полное (!) отключение потому и фактически return
  // и код дальше выполняться не будет
  // то есть после просыпания код начнётся выполняться как с самого начала
  // и сработает естественная инициализация вайфая, как при первом запуске

  Serial.println();
  initWiFi();

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println(WiFi.softAPmacAddress());
    Serial.println(WiFi.softAPIP());
  }
  else
  {
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.RSSI());
  }
  WiFi.printDiag(Serial);
}

void loop()
{
}
