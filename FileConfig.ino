// Загрузка данных сохраненных в файл  config.json

bool loadConfig() {
  // Открываем файл для чтения
  File configFile = SPIFFS.open("/configs.json", "r");
  if (!configFile) {
  // если файл не найден
    Serial.println("Failed to open config file");
  //  Создаем файл запиав в него аные по умолчанию
    saveConfig();
    configFile.close();
    return false;
  }
  // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    configFile.close();
    return false;
  }

// загружаем файл конфигурации в глобальную переменную
  jsonConfig = configFile.readString();
  configFile.close();
  // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
    DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  //  строку возьмем из глобальной переменной String jsonConfig
    JsonObject& root = jsonBuffer.parseObject(jsonConfig);
  // Теперь можно получить значения из root
    _ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
    _passwordAP = root["ssidAPPassword"].as<String>();
    timezone = root["timezone"];               // Так получаем число
    r = root["rVal"];
    g = root["gVal"];
    b = root["bVal"];

    r1 = root["r1Val"];
    g1 = root["g1Val"];
    b1 = root["b1Val"];
    
    r2 = root["r2Val"];
    g2 = root["g2Val"];
    b2 = root["b2Val"];

    SecondZoneMode=root["cM1"];
    UseStripAsActivityLed=root["usaled"];

    MQTT_IDX = root["idx"];
    SSDP_Name = root["SSDPName"].as<String>();
    _ssid = root["ssidName"].as<String>();
    _password = root["ssidPassword"].as<String>();
    CurrentLedMode=root["cM"];
    
    TOTALPIXELS = root["pnum"];
    EFFECTSPIXELS = root["peffects"];

    IOLEDPIN = root["IOLEDPIN"];
    BOOTLEDPIN = root["BOOTLEDPIN"];
    RELAYPIN4 = root["RELAYPIN4"];
    BUTTONPIN = root["BUTTONPIN"];
    PWMPIN = root["PWMPIN"];
    STRIPPIN = root["STRIPPIN"];
    PIN_SDA = root["PIN_SDA"];
    PIN_SCL = root["PIN_SCL"];
    IO_inversed = root["IO_inversed"];
    BOOT_inversed = root["BOOT_inversed"];
    //mdivider = root["pdiv"];
    //Z_FIRSTEND = EFFECTSPIXELS;
    //mstep = EFFECTSPIXELS/mdivider;
    return true;
}

// Запись данных в файл config.json
bool saveConfig() {
  BootBlink(50);
  // Резервируем память для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);
  // Заполняем поля json
  json["SSDPName"] = SSDP_Name;
  json["ssidAPName"] = _ssidAP;
  json["ssidAPPassword"] = _passwordAP;
  json["ssidName"] = _ssid;
  json["ssidPassword"] = _password;
  json["timezone"] = timezone;
  json["rVal"] = r;
  json["gVal"] = g;
  json["bVal"] = b;
  json["r2Val"] = r2;
  json["g2Val"] = g2;
  json["b2Val"] = b2;
  json["cM"] = CurrentLedMode;
  json["usaled"] = UseStripAsActivityLed;
  json["r1Val"] = r1;
  json["g1Val"] = g1;
  json["b1Val"] = b1;
  json["cM1"] = SecondZoneMode;

  json["idx"] = MQTT_IDX;
  json["pnum"] = TOTALPIXELS;
  json["peffects"] = EFFECTSPIXELS;
  //json["pdiv"] = mdivider;

  json["IOLEDPIN"]=IOLEDPIN;
  json["BOOTLEDPIN"]=BOOTLEDPIN;
  json["RELAYPIN4"]=RELAYPIN4;
  json["BUTTONPIN"]=BUTTONPIN;
  json["PWMPIN"]=PWMPIN;
  json["STRIPPIN"]=STRIPPIN;
  json["PIN_SDA"]=PIN_SDA;
  json["PIN_SCL"]=PIN_SCL;
  json["IO_inversed"]=IO_inversed;
  json["BOOT_inversed"]=BOOT_inversed;

  // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  json.printTo(jsonConfig);
  // Открываем файл для записи
  File configFile = SPIFFS.open("/configs.json", "w");
  delay(5);
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    configFile.close();
    return false;
  }
  // Записываем строку json в файл
  String jsonStr;
  json.printTo(jsonStr);
  Serial.println(jsonStr);
  json.printTo(configFile);
  delay(20);
  configFile.close();
  delay(5);
  BootBlink(50);
  return true;
  }
  
