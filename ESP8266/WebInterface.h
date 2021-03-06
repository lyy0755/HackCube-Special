
String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  else if (filename.endsWith(".woff2")) return "application/octet-stream";
  return "text/plain";
}

bool handleFileRead(String path) {
#ifdef DEBUG
  Serial.println("handleFileRead: " + path);
#endif
  if (path.endsWith("/")) path += "RF.html";
  if (SPIFFS.exists(path)) {
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, getContentType(path));
    file.close();
    return true;
  }
  return false;
}


void WebInterface() {

  /*
      WIFI Interface
  */
  server.on("/", []() {
    String data = "";
    data = "[HID]" + server.arg("HidData");
    Serial.println(data);
    LED_STATE(LED_TRANSMIT, true);
    handleFileRead("/NFC.html");
  });

  server.on("/APScanResults.json", sendAPResults);
  server.on("/APScan.json", startAPScan);
  server.on("/APSelect.json", selectAP);
  server.on("/getResultsJSON.json", ResultsJSON);


  server.on("/rf.json", rf_json);
  server.on("/nfc.json", nfc_json);
  server.on("/nfc_simulate", []() {
    int VD = server.arg("VD").toInt();
    unsigned long  ID = strtoul(server.arg("ID").c_str(), NULL, 10);
    Serial.print("[NFC][simulation]vd:");
    Serial.print(VD);
    Serial.print(",data:");
    Serial.println(ID);
    //rf_Replay(num);
    server.send(200, "text/html", "");
    LED_STATE(LED_SIMULATE);
  });

  server.on("/nfc_stop", []() {
    Serial.println("[NFC]stop");
    server.send(200, "text/html", "");
    LED_STATE(LED_RUN);
  });
  server.on("/nfc_read", []() {
    Serial.print("[NFC][switch]action:");
    Serial.println(server.arg("action"));
    server.send(200, "text/html", "");
    LED_STATE(LED_RUN);
  });

  //nfc_clear
  server.on("/nfc_clear", []() {
    NFCnum = 0;
    server.send(200, "text/html", "nfc clear ok!");
  });
  
  server.on("/nfc_attack", []() {
    int VD = server.arg("vd").toInt();
    unsigned long  START = strtoul(server.arg("start").c_str(), NULL, 10);
    unsigned long  END = strtoul(server.arg("end").c_str(), NULL, 10);
    Serial.print("[NFC][attack]vd:");
    Serial.print(VD);
    Serial.print(",start:");
    Serial.print(START);
    Serial.print(",end:");
    Serial.println(END);
    //rf_Replay(num);
    //LED_Color(255, 0, 0);
    LED_STATE(LED_ATTACK);
    server.send(200, "text/html", "");
  });


  server.on("/rf_power", []() {
    Serial.print("[RF][Switch]action:");
    Serial.println(server.arg("action"));
    server.send(200, "text/html", "");
    LED_STATE(LED_RUN);
  });

    server.on("/rf_setup", []() {
    Serial.print("[RF][Setup]chip:CC1101,class:");
    Serial.print(server.arg("SnifferClass"));
    Serial.print(",freq:");
    Serial.println(server.arg("freq"));
    
    server.send(200, "text/html", "");
    LED_STATE(LED_RUN);
  });
  

  server.on("/rf_jam", []() {
    //Serial.println("[NFC]stop");
    String Jam_class = server.arg("class");
    if (Jam_class == "Smart") {
      Serial.print("[RF][Jam]class:Smart,freq1:");
      Serial.print(server.arg("freq1"));
      Serial.print(",freq2:");
      Serial.print(server.arg("freq2"));
    } else if (Jam_class == "Rude") {
      Serial.print("[RF][Jam]class:Rude,freq:");
      Serial.print(server.arg("freq"));
    }
    Serial.println("");
    server.send(200, "text/html", "");
    LED_STATE(LED_ATTACK);
  });

  server.on("/rf_jam_stop", []() {
    Serial.println("[RF][Jam]Stop");
    server.send(200, "text/html", "");
    LED_STATE(LED_RUN);
  });


  server.on("/rf_Replay", []() {
    
    int num = server.arg("num").toInt();
    rf_Replay(num);
    server.send(200, "text/html", "");
    LED_STATE(LED_TRANSMIT, true);
    
  });


  server.on("/rf_Transmit", []() {
    //int num = server.arg("num").toInt();
    String Freq = server.arg("Freq");
    String Data = server.arg("Data");
    String Func = server.arg("Func");
    String Class = server.arg("Class");
    Serial.print("[RF][Transmit]");
    Serial.print(Freq);
    Serial.print(",");
    Serial.print(Func);
    Serial.print(",");
    Serial.println(Data);

//    ajaxData_nfc = analogRead(A0);
    
    server.send(200, "text/html", "True");
    LED_STATE(LED_TRANSMIT, true);
  });

   server.on("/rf_LigthBar", []() {
    //int num = server.arg("num").toInt();
    String Freq = server.arg("Freq");
    String Data = server.arg("Data");
    Serial.print("[RF][Transmit]LigthBar,freq:");
    Serial.print(Freq);
    Serial.print(",data:");
    Serial.println(Data);
    server.send(200, "text/html", "True");
    LED_STATE(LED_TRANSMIT, true);
  });
  

  server.on("/ajax_nfc", []() {
    server.send(200, "text/html", getajax_nfc());
    //server.send(200, "text/html","12312312");
    //Serial.println("ajaxData_nfc Send");
    ajaxData_nfc = "";
  });

  server.on("/ajax_rf_test", []() {
    server.send(200, "text/html", getajax_rf());
    ajaxData_rf = "";
  });

  server.on("/ajax_rf_24l01", []() {
    server.send(200, "text/html", getajax_rf_24l01());
    ajaxData_rf_24l01 = "";
  });

  server.on("/ajax_rf_prog", []() {
    server.send(200, "text/html", getrf_prog());
    rf_prog = "";
  });

  server.on("/ajax_rf_jam", []() {
    server.send(200, "text/html", getrf_jam());
    rf_jam = "";
  });

  server.on("/ajax_nfc_prog", []() {
    server.send(200, "text/html", getnfc_prog());
    nfc_prog = "";
  });

  server.on("/rf_blast_c2", []() {
    led_action = 0;
    LED_STATE(LED_ATTACK);
    //Serial.println("rf_blast_c2");
    //rc2start1755end17a5
    Serial.print("[RF][Attack]Class2,");
    Serial.print("freq:");
    Serial.print(server.arg("freq"));
    Serial.print(",start:");
    Serial.print(server.arg("start"));
    Serial.print(",end:");
    Serial.print(server.arg("end"));
    Serial.print(",func:");
    Serial.println(server.arg("func"));
    server.send(200, "text/html", "True");
    //handleFileRead("/RF.html");
  });

  server.on("/tpms", []() {
    LED_STATE(LED_TRANSMIT, true);
    byte tpmsdata[4][11];
    if (server.arg("tpms[0][0]").length() > 0) {
      for (int a = 0; a < 4; a++) {
        unsigned long id = strtoul(server.arg("tpms[" + String(a) + "][0]").c_str(), NULL, 16);
        for (int i = 0; i < 8; i++) {
          if (i < 4) {
            tpmsdata[a][i] = (id << i * 8) >> 24 & 0xff;
          } else {
            tpmsdata[a][i] = strtoul(server.arg("tpms[" + String(a) + "][" + String (i) + "]").c_str(), NULL, 16);
          }
        }
      }
      for (int a = 0; a < 4; a++) {
        Serial.print("[RF][TPMS]");
        for (int i = 0; i < 8; i++) {
          Serial.print(tpmsdata[a][i], HEX);
        }
        Serial.println("");
        delay(800);
      }
    }
  });

  server.on("/HID_attack", []() {
    String data = "";
    data = "[HID]" + server.arg("HidData");
    Serial.println(data);
    LED_STATE(LED_TRANSMIT, true);
    server.send(200, "text/html", "true");
    //handleFileRead("/HID.html");
  });

  server.on("/solidColor", []() {
    String color = server.arg("color");
    led_action = server.arg("action").toInt();
    color = color.substring(1, color.length());
    long  color16 = strtoul(color.c_str(), NULL, 16);
    //Serial.println(color16, HEX);
    int r = (color16 >> 16) & 0xff ;
    int g = (color16 >> 8) & 0xff;
    int b = color16 & 0xff;
    LED_Color(r, g, b);
    int brightness = server.arg("brightness").toInt();
    if (brightness)
      Brightnessset(brightness);
    server.send(200, "text/html", "true");
    //handleFileRead("/INFO.html");
  });

  server.on("/ipconfig", []() {
    String ip = server.arg("ip");
    String subnet = server.arg("subnet");
    server.send(200, "text/html", "true");
    doc["ip"] = ip;
    doc["subnet"] = subnet;
    String jsonConfig = "/config.json";

    //File file = SD.open(jsonConfig.c_str());

    if (!SPIFFS.exists(jsonConfig)) {
      Serial.println(F(sprintf("ERROR: config file '%s' is NOT found!!!", jsonConfig.c_str())));
    }
    File file = SPIFFS.open(jsonConfig, "w");
    if (serializeJson(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    file.close();
    Serial.println(ips);
    ESP.restart();
  });

  server.on("/wificonfig", []() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    server.send(200, "text/html", "true");
    doc["ssid"] = ssid;
    doc["password"] = password;
    String jsonConfig = "/config.json";

    //File file = SD.open(jsonConfig.c_str());

    if (!SPIFFS.exists(jsonConfig)) {
      Serial.println(F(sprintf("ERROR: config file '%s' is NOT found!!!", jsonConfig.c_str())));
    }
    File file = SPIFFS.open(jsonConfig, "w");
    if (serializeJson(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    file.close();
    Serial.println(ssid);
    ESP.restart();
  });

  server.on("/apwificonfig", []() {
    String apssid = server.arg("apssid");
    String appassword = server.arg("appassword");
    server.send(200, "text/html", "true");
    doc["apssid"] = ssid;
    doc["appassword"] = password;
    String jsonConfig = "/config.json";

    //File file = SD.open(jsonConfig.c_str());

    if (!SPIFFS.exists(jsonConfig)) {
      Serial.println(F(sprintf("ERROR: config file '%s' is NOT found!!!", jsonConfig.c_str())));
    }
    File file = SPIFFS.open(jsonConfig, "w");
    if (serializeJson(doc, file) == 0) {
      Serial.println(F("Failed to write to file"));
    }
    // Close the file
    file.close();
    Serial.println(apssid);
    ESP.restart();
  });

server.on("/restart", []() {
    Serial.println("restart ok!");
    ESP.restart();
  });
  
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "Archivo no encontrado");
  });
  
}
