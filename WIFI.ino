void WIFIinit() {
  String pb = "WiFi"; 
  // Попытка подключения к точке доступа
  WiFi.mode(WIFI_STA);
  byte tries = 11;
  WiFi.begin(_ssid.c_str(), _password.c_str());
  // Делаем проверку подключения до тех пор пока счетчик tries
  // не станет равен нулю или не получим подключение
  
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    pb+=".";
    drawMode(pb, "", -100, true);
    BootBlink(50);
    delay(1000);
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    // Если не удалось подключиться запускаем в режиме AP
    Serial.println("");
    Serial.println("WiFi up AP");
    drawMode("Mode AP", "", -100, true);
    StartAPMode();
    boolean APMode = true;
  }
  else {
    // Иначе удалось подключиться отправляем сообщение
    // о подключении и выводим адрес IP
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    drawMode("Mode STA", "", -100, true);
    char buf[16];
    sprintf(buf, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
    myIP = buf;
    boolean APMode = false;
    drawMode(String(buf), "", -100, true);
//    drawMode("IP:" + String(WiFi.localIP()[3]));
    //drawIP();
  }
}

bool StartAPMode() {
  /*IPAddress apIP(192, 168, 4, 1);
  // Отключаем WIFI
  WiFi.disconnect();
  // Меняем режим на режим точки доступа
  WiFi.mode(WIFI_AP);
  // Задаем настройки сети
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  // Включаем WIFI в режиме точки доступа с именем и паролем
  // хронящихся в переменных _ssidAP _passwordAP
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());
  return true;*/
  WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str());

  IPAddress mysoftIP = WiFi.softAPIP();
  char buf[16];
  sprintf(buf, "IP:%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] );
  myIP = buf;
  Serial.print("AP IP address: ");
  Serial.println(mysoftIP);
  return true;
}
