#include "stdlib_noniso.h"
#include "wificonfig.h"

const bool debug = true;
void sendHeader(int code, String type, size_t _size) {
  server.setContentLength(_size);
  server.send(code, type, "");
  Serial.print("setsize");
  Serial.println(_size);
}

#define bufSize 2500
int bufc = 0; //buffer counter;
char data_websiteBuffer[bufSize];

void startAPScan() {
  //scanMode = "scanning...";
  if (apScan.start()) {
    Serial.println("scanning");
    // apScan.sort();
    server.send ( 200, "text/json", "true");
    //    attack.stopAll();
    //    scanMode = "SCAN";
  }
  Serial.println("scanend");
}
void sendBuffer() {
  if (bufc > 0) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
}
void sendToBuffer(String str) {
  size_t len = str.length();
  if (bufc + len > bufSize) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
  memcpy(data_websiteBuffer + bufc, str.c_str(), len);
  bufc += len;
}

void sendAPResults() {
  apScan.sendResults();
}

void selectAP() {
  if (server.hasArg("num")) {
    apScan.select(server.arg("num").toInt());
  }
}
void ResultsJSON() {
  apScan.getResultsJSON();
}

void ConnectWif() {
  WiFi.begin(apname, appassword);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void ConfigWifi() {
  IPAddress local_IP(192, 168, 36, 1);
  IPAddress gateway(192, 168, 36, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.mode(WIFI_AP);
  Serial.print("SSID:");
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  delay(100);
}

void connectWifi() {
  unsigned long outtime = millis();
  bool outflag = false;
  WiFi.mode(WIFI_STA);
 WiFi.begin(apname, appassword);
  while (WiFi.status() != WL_CONNECTED && !outflag) {
    delay(500);
    Serial.print(".");
    if (millis() - outtime > 10000)
      outflag = true;
  }
  if (outflag) {
    ConfigWifi();
    Serial.println("out");
  } else {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
