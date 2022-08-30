
void HTTP_init(void) {

  HTTP.on("/config.json", handle_ConfigJSON); // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  HTTP.on("/ssdp", handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid", handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/ssidap", handle_Set_Ssidap); // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/restart", handle_Restart);   // Перезагрузка модуля по запросу вида /restart?device=ok
  HTTP.on("/setrgb", handle_Set_RGB);
  HTTP.on("/setmode", handle_Set_Mode);
  HTTP.on("/set2mode", handle_Set_2th_Zone);
  HTTP.on("/set3mode", handle_Set_3th_Zone);
  HTTP.on("/set2modeoff", handle_Off_2th_Zone);
  HTTP.on("/setidx", handle_Set_Idx);
  HTTP.on("/setlen", handle_Set_Length);
  HTTP.on("/io", handle_Set_IO);
  
  // Добавляем функцию Update для перезаписи прошивки по WiFi при 1М(256K SPIFFS) и выше
  httpUpdater.setup(&HTTP);
  // Запускаем HTTP сервер
  HTTP.begin();
}

void handle_Set_IO()
{
  setIO();
  saveConfig();                        // Функция сохранения данных во Flash  
  drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true);
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_3th_Zone()
{
  r2 = HTTP.arg("r2Val").toInt();
  g2 = HTTP.arg("g2Val").toInt();
  b2 = HTTP.arg("b2Val").toInt();
  UseStripAsActivityLed = HTTP.arg("usaled").toInt();
  saveConfig();                        // Функция сохранения данных во Flash  
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_2th_Zone()
{
  r1 = HTTP.arg("r1Val").toInt();
  g1 = HTTP.arg("g1Val").toInt();
  b1 = HTTP.arg("b1Val").toInt();
  Serial.print("r1: ");
  Serial.println(r1);
  Serial.print("g1: ");
  Serial.println(g1);
  Serial.print("b1: ");
  Serial.println(b1);  
  SecondZoneMode = true;
  saveConfig();                        // Функция сохранения данных во Flash  
  ModeSimpleSecondZone();
  drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true);
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Off_2th_Zone()
{
  SecondZoneMode = false;
  saveConfig();   
  ModeOffSecondZone();
  HTTP.send(200, "text/plain", "OK");
}

void handle_Set_Ssdp() {
  SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
  saveConfig();                 // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
}

void handle_Set_Length() {
  TOTALPIXELS = HTTP.arg("pnum").toInt();            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  EFFECTSPIXELS = HTTP.arg("peffects").toInt();
  //mdivider = HTTP.arg("pdiv").toInt();    // Получаем значение password из запроса сохраняем в глобальной переменной
  //Z_FIRSTEND = EFFECTSPIXELS;  
  //mstep = EFFECTSPIXELS/mdivider;
  Serial.println(TOTALPIXELS);
  //Serial.println(mdivider);
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении  
}
// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  _ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  Serial.println(_ssid);
  Serial.println(_password);
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_Idx() {
  MQTT_IDX = HTTP.arg("idx").toInt();            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  SubscribeTopics();
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_Mode()
{
  CurrentLedMode = HTTP.arg("mode").toInt();
  changeMode(CurrentLedMode);
  saveConfig();                        // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

void handle_Set_RGB() {
  r = HTTP.arg("rVal").toInt();
  g = HTTP.arg("gVal").toInt();
  b = HTTP.arg("bVal").toInt();
  Serial.print("r: ");
  Serial.println(r);
  Serial.print("g: ");
  Serial.println(g);
  Serial.print("b: ");
  Serial.println(b);
  saveConfig();                        // Функция сохранения данных во Flash
  changeMode(MODE_SIMPLE);
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}

//Установка параметров внутренней точки доступа по запросу вида http://192.168.0.101/ssidap?ssidAP=home1&passwordAP=8765439
void handle_Set_Ssidap() {              //
  _ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}
// Перезагрузка модуля по запросу вида http://192.168.0.101/restart?device=ok
void handle_Restart() {
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
  }
}

void handle_ConfigJSON() {
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  json["SSDP"] = SSDP_Name;
  json["ssidAP"] = _ssidAP;
  json["passwordAP"] = _passwordAP;
  json["ssid"] = _ssid;
  json["password"] = _password;
  json["timezone"] = timezone;
  json["ip"] = WiFi.localIP().toString();
  json["time"] = GetTime();
  json["date"] = GetDate();
  json["rVal"] = r;
  json["gVal"] = g;
  json["bVal"] = b;
  json["r1Val"] = r1;
  json["g1Val"] = g1;
  json["b1Val"] = b1;
  json["r2Val"] = r2;
  json["g2Val"] = g2;
  json["b2Val"] = b2;
  json["cM"] = CurrentLedMode;
  json["cM1"] = SecondZoneMode;
  json["usaled"] = UseStripAsActivityLed;
  json["idx"] = MQTT_IDX;
  json["pnum"] = TOTALPIXELS;
  json["peffects"] = EFFECTSPIXELS;
  json["uptime"] = uptime();
  
  json["IOLEDPIN"]=IOLEDPIN;
  json["BOOTLEDPIN"]=BOOTLEDPIN;
  json["RELAYPIN4"]=RELAYPIN4;
  json["BUTTONPIN"]=BUTTONPIN;
  json["PWMPIN"]=PWMPIN;
  json["STRIPPIN"]=STRIPPIN;
  json["TAHOPIN"]=TAHOPIN;
  json["PIN_SDA"]=PIN_SDA;
  json["PIN_SCL"]=PIN_SCL;
  json["IO_inversed"]=IO_inversed;
  json["BOOT_inversed"]=BOOT_inversed;
  //json["pdiv"] = mdivider;
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  HTTP.send(200, "text/json", root);
}
