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
