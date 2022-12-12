#include <Arduino.h>
#include <ESP8266WiFi.h>
const char *ssid_STA = "nikelborm";
const char *password_STA = "12345678";

const char *ssid_AP = "ESP Web-server";
const char *password_AP = "12345678";

const char *ebuchegoRazmeraManifestjson = "{\"payload\":{\"shortname\":\"door locked/unlocked open/close\",\"fullname\":\"Механизм открытия/закрытия дверей с обратной связью\",\"description\":\"Механизм, который позволяет разблокировать дверь по команде и заблокировать дверь по команде, а также узнать дверь сейчас открыта или закрыта\",\"UUID\":\"51eea88c-9c7c-49a5-b38c-a1526ac88e0f\",\"encryptionModuleUUID\":\"ca4e23ec-f2a4-4d78-aa94-2065d72d5824\",\"encryptionModuleCredentials\":{\"clientPublicKey\":\"public client key of esp8266\",\"clientUUIDSignedByClientPrivateKey\":\"blablablablablabalblalbabalbbalablablalbablaabllbaablabalbalablablablabalb\"},\"supported\":{\"eventParameters\":[{\"UUID\":\"b0f1315d-b681-4a39-b50c-ad30b0740d96\",\"name\":\"Флаг: Должна ли быть заблокирована дверь?\",\"dataValidatorUUID\":\"930877ce-d692-4ae1-a1db-580ae6546c36\",\"measurementUnit\":\"Булево значение\"}],\"events\":[{\"UUID\":\"344fe486-4006-445c-ab92-84cb427e2d7e\",\"name\":\"Заблокировать дверь\",\"description\":\"Команда, которая приводит к блокировке двери\",\"type\":\"command\",\"requiredParameterUUIDs\":[],\"optionalParameterUUIDs\":[],\"hexColor\":\"1abd23\"},{\"UUID\":\"08ff24dd-1cc8-424a-a638-88e68cf4218e\",\"name\":\"Разблокировать дверь\",\"description\":\"Команда, которая приводит к разблокировке двери\",\"type\":\"command\",\"requiredParameterUUIDs\":[],\"optionalParameterUUIDs\":[],\"hexColor\":\"123abd\"},{\"UUID\":\"adcb7802-a91c-44cb-8404-fd01d2af4624\",\"name\":\"Установить статус блокировки двери\",\"description\":\"Команда, которая приводит к установки статуса блокировки двери\",\"type\":\"command\",\"requiredParameterUUIDs\":[\"b0f1315d-b681-4a39-b50c-ad30b0740d96\"],\"optionalParameterUUIDs\":[],\"hexColor\":\"126067\"},{\"UUID\":\"88658487-39c3-484e-81d3-84c432d72d44\",\"name\":\"Дверь Открылась\",\"description\":\"Лог о том, что открылась дверь\",\"type\":\"log\",\"requiredParameterUUIDs\":[],\"optionalParameterUUIDs\":[],\"hexColor\":\"0e8203\"},{\"UUID\":\"a04a2a11-2f2d-4f5e-9659-7337f4543f7a\",\"name\":\"Дверь закрылась\",\"description\":\"Лог о том, что закрылась дверь\",\"type\":\"log\",\"requiredParameterUUIDs\":[],\"optionalParameterUUIDs\":[],\"hexColor\":\"10128d\"}],\"routeEndpoints\":[{\"UUID\":\"0578d33e-0f83-411d-91e8-a6eb3add4432\",\"name\":\"Unlock door endpoint\",\"shortcode\":\"UD\",\"description\":\"Вход для команды на разблокировку двери\",\"eventUUID\":\"08ff24dd-1cc8-424a-a638-88e68cf4218e\",\"type\":\"eventSink\",\"hexColor\":\"077291\"},{\"UUID\":\"c2d4e359-1171-44c0-b1c7-4ab44b9de44a\",\"name\":\"Lock door endpoint\",\"shortcode\":\"LD\",\"description\":\"Вход для команды на блокировку двери\",\"eventUUID\":\"344fe486-4006-445c-ab92-84cb427e2d7e\",\"type\":\"eventSink\",\"hexColor\":\"169420\"},{\"UUID\":\"98e6e883-6b15-46e5-9fc8-9aef8116e47c\",\"name\":\"Set door status endpoint\",\"shortcode\":\"SDS\",\"description\":\"Вход для команды на установку статуса блокировки двери\",\"eventUUID\":\"adcb7802-a91c-44cb-8404-fd01d2af4624\",\"type\":\"eventSink\",\"hexColor\":\"845130\"},{\"UUID\":\"db414f8b-a7b9-4c94-b00e-c0f9890e1b8e\",\"name\":\"Door was opened\",\"shortcode\":\"OD\",\"description\":\"Выход с сообщениями об открытой двери\",\"eventUUID\":\"88658487-39c3-484e-81d3-84c432d72d44\",\"type\":\"eventSource\",\"hexColor\":\"160420\"},{\"UUID\":\"646014f8-d864-462e-a49c-6209f083884c\",\"name\":\"Door was closed\",\"shortcode\":\"CD\",\"description\":\"Выход с сообщениями о закрытой двери\",\"eventUUID\":\"a04a2a11-2f2d-4f5e-9659-7337f4543f7a\",\"type\":\"eventSource\",\"hexColor\":\"840130\"}],\"transport\":{\"wss\":true,\"http\":false}}}}";

String serialData = "";
char charBuf[80];
String memoryData = "";

uint addr = 0;

struct
{
  uint val = 0;
  char str[80] = "";
} data;

void setup()
{

  Serial.begin(115200);
}

void loop()
{
  if (!Serial.available())
    return;
  //Читаю с порта
  serialData = Serial.readString();
  Serial.flush();
  serialData.toCharArray(charBuf, 80);
  //Форматирую строку
  // serialData = serialData.substring(0, serialData.length()-1);
  //Работаю с оперативной памятью перед загрузкой
  strncpy(data.str, charBuf, 80);
  delay(5000);
  Serial.print("{" + String(data.str).substring(0, serialData.length() - 1) + "}");
}
