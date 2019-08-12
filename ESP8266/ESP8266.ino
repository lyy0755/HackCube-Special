/*
                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@             
                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@            
               @@@@@@@@@                    @@@@           
             @@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@         
            @@@@@   @@@@@@@@@@@@@@@@@@@@@@@   @@@@@        
           @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@       
          @@@@@   @@@@@    @@@@@@@@@@@@@@@@@@   @@@@@      
         @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@     
        @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@    
      @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@  
     @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@ 
    @@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@@
    @@@@@                       @@@@@@@@@@@@@@@@@@@@  @@@@@
     @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@ 
      @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@  
       @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@   
         @@@@   @@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@     
          @@@@   @@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@      
           @@@@    @@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@       
            @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@@@        
             @@@@@   @@@@@@@@@@@@@@@@@   @@@@@@@@@         
              @@@@@                       @@@@@@@          
               @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@           
                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@             
       _                   _          _                  
      | |                 | |        | |                 
      | |__    __ _   ___ | | __ ___ | |__   _   _   ___ 
      | '_ \  / _` | / __|| |/ // __|| '_ \ | | | | / _ \
      | | | || (_| || (__ |   <| (__ | |_) || |_| ||  __/
      |_| |_| \__,_| \___||_|\_\\___||_.__/  \__,_| \___|
                                                                                      
    Author: mobier
    Email: vxxwej@gmail.com
    Git repository: https://github.com/UnicornTeam/HackCube-Special
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "APScan.h" //https://github.com/samdenty99/Wi-PWN
#include <Hash.h>
#include <FS.h>

MDNSResponder mdns;
APScan apScan;

//#define DEBUG;
ESP8266WebServer server(80);

#include "LED.h"
#include "JSON.h"
#include "WEB.h"
#include "SerialCommunication.h"
#include "WIFI.h"
#include "WebInterface.h"

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  LED_STATE(LED_CONNECT);
  SPIFFS.begin();
  delay(100);
  readConfig();
  delay(100);
  ConfigWifi(); //建立"XXXXXX"热点
  //ConnectWif();
  delay(100);
  LED_STATE(LED_RUN);
  Serial.swap(); //将串口切换到和ATmega32u4通信串口中
  delay(100);
  WebInterface(); //初始化网页端接口
  server.begin(); 
  delay(100);
  led_action = 1;
  delay(100);
}


void loop() {
  SerialCmd();
  LowBatteryCheck();
  LED_SHOW();
  server.handleClient();
}
