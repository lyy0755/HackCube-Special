#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson

char ssid[32];
char password[32];
char apname[32];
char appassword[32];
char ips[32];
char subnets[32];
StaticJsonDocument<512> doc;

void readConfig(){
  String jsonConfig = "/config.json";

  //File file = SD.open(jsonConfig.c_str());

  if (!SPIFFS.exists(jsonConfig)) {
    Serial.println(F(sprintf("ERROR: config file '%s' is NOT found!!!", jsonConfig.c_str())));
  }
  File file = SPIFFS.open(jsonConfig, "r");
  
  //反序列化json
  DeserializationError error = deserializeJson(doc, file);
  if (error)
    Serial.println(F("Failed to read file, using default configuration"));
  
  //读取值
  strlcpy(ssid,doc["ssid"] | "hackcube",sizeof(ssid));
  strlcpy(password,doc["password"] | "hackcube",sizeof(password)); 
  strlcpy(apname,doc["apname"] | "hackcube",sizeof(apname)); 
  strlcpy(appassword,doc["appassword"] | "hackcube",sizeof(appassword));
  strlcpy(ips,doc["ip"] | "192.168.1.1",sizeof(ips));
  strlcpy(subnets,doc["subnet"] | "255.255.255.0",sizeof(subnets));
  
}
