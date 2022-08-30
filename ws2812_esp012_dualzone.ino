
#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>   
#include <ESP8266SSDP.h>       
#include <FS.h>                 
#include <ArduinoJson.h>        
#include <ESP8266HTTPUpdateServer.h>  //Содержится в пакете.
#include <Ticker.h>  
#include <Adafruit_NeoPixel.h>


ESP8266HTTPUpdateServer httpUpdater;
ESP8266WebServer HTTP(80);
File fsUploadFile;
Ticker blinker;
Ticker taho;
//Ticker tpwm;

volatile boolean NeedUpdateBMP=false;

String myIP = "";

int MQTT_IDX = 29;
int TOTALPIXELS = 10;
int EFFECTSPIXELS = 8;

boolean BMP_enable = true;
boolean APMode = false;

String jsonConfig = "{}";
int r, g, b, j, r1, g1, b1, r2, g2, b2  = 0;
int delayval = 50;

const int MODE_SIMPLE = 0;
const int MODE_ALL = 1;
const int MODE_RAINBOW = 2;
const int MODE_FLAME = 4;
const int MODE_THEATER = 3;
const int MODE_NIGHT = 5;
const int MODE_OFF = 6;
const int MODE_OFF_DUAL = 7;
const int MODE_OFF_ALL = 8;

int CurrentLedMode = MODE_OFF;
int SecondZoneMode = 0;
int CurrentPWM = 0;

int IOLEDPIN=4;
int BOOTLEDPIN=14;
int RELAYPIN4=4;
int BUTTONPIN=14;
int PWMPIN=12;
int TAHOPIN=13;
int STRIPPIN = 0;

int PIN_SDA = 2;
int PIN_SCL = 5;

int IO_inversed = 1;
int BOOT_inversed = 0;
boolean UseStripAsActivityLed = true;

Adafruit_NeoPixel strip;

volatile boolean needUpdate = true;
volatile boolean setBreak = false;
volatile boolean _period = false;
volatile int val;
volatile int duty=0;
volatile int sPwmCounter=0;

float tempC = -100.0;

// Определяем переменные wifi
String _ssid     = "samalet"; // Для хранения SSID
String _password = "samalet123"; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String SSDP_Name = "Update-Time"; // Имя SSDP
String MQTT_server = "fs6.umodom.ru";
String MQTT_user = "maxim";
String MQTT_password = "ctdfcnjgjkm";
int MQTT_port = 1883;               
int timezone = 3;               
boolean EnablePWM = false;      

void TimerUpdate()
{
  NeedUpdateBMP = true;  
}

void rpm()
{
  val++;
}

void TahoUpdate()
{
  _period = !_period;  
  if (!_period)
  {
    CurrentPWM=val*30;
  }
  else
  {
    val=0;
  }
}



void ICACHE_RAM_ATTR rpm();

void setup() {  
  Serial.begin(115200);
  //Serial.println("");
  //drawMode("FS Init...", "", -100, true);
  FS_init();
  BootBlink(50);
  
  loadConfig();    
  delay(100);
  strip = Adafruit_NeoPixel(TOTALPIXELS, STRIPPIN, NEO_GRB + NEO_KHZ800);
  setup_pwm();
  Strip_INIT();
  BootBlink(50);
  InitSSD1306();
  drawLogo();
  delay(3000);
  BootBlink(50);
  drawSmallLogo();
  
  delay(100);
  drawMode("Config...", "", -100, true);
  BootBlink(50);
 /* loadConfig();    
  delay(100);*/
  BootBlink(50);
  drawMode("I/O", "", -100, true);
  IOSetup();
  delay(100);
  BootBlink(50);
  drawMode("WiFi...", "", -100, true);
  WIFIinit();
  delay(1000);
  BootBlink(50);
  drawMode("Web Server", "", -100, true);
  HTTP_init();
  delay(100);
  BootBlink(50);
  drawMode("Time Sync", "", -100, true);
  Time_init();
  
  /*SSDP_init();*/
  delay(100);
  BootBlink(50);
  
  drawMode("Cloud...", "", -100, true);  
  Effects_INIT();
  delay(300);
  BootBlink(50);
  BootBlink(50);
  BootBlink(50);
  BootBlink(50);
  BootBlink(50);
  BootBlink(50);
  //return;
  drawMode("Sensors", "", -100, true);  
  BMP_init(); 
  delay(300);
  BootBlink(50);
  BootBlink(50);
  drawMode("Informer", "", -100, true);  
  mail_send("boot");
  delay(300);
  BootBlink(50);
  drawMode("Wake Up...", "", -100, true);  
  WakeUp();
  blinker.attach(5, TimerUpdate);
  taho.attach(0.5, TahoUpdate);
  //tpwm.attach_ms(1, sPwmUpdate);
  
  BootBlink(100);
  BootBlink(50);
}

void PWMControl()
{
  if (tempC < 30)
  {
    SetPWM(200);
  }  
  else
  if ((tempC > 30) && (tempC <= 31))
  {
    SetPWM(350);
  }
  else
  if ((tempC > 31) && (tempC <= 32))
  {
    SetPWM(500);
  }
  else
  if ((tempC > 32) && (tempC <= 33))
  {
    SetPWM(700);
  }
  else
  if (tempC > 34)
  {
    SetPWM(1000);
  }
  
  
}

void loop() {
  HTTP.handleClient();
  if (APMode) 
  {
    BlinkAPMode();
    return;
  }
  Effects_loop();
  if (NeedUpdateBMP)
  {
    NeedUpdateBMP=false;
    BMP_loop();
    //PWMControl();
    //_pwm(tempC);
  }
  //UpdatePWM();
  //ESP.wdtFeed();
  //delay(1);
}
