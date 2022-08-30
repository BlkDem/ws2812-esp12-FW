
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <ArduinoJson.h>

const char* host = "strip29";

#define BRIGHTNESS     50
#define DNS = "8.8.8.8"

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, Str2Char(MQTT_server), 1883, Str2Char(MQTT_user), Str2Char(MQTT_password));



Adafruit_MQTT_Subscribe slider0 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/nvalue");
Adafruit_MQTT_Subscribe color0 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/color");
Adafruit_MQTT_Subscribe color1 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/color1");
/*Adafruit_MQTT_Subscribe color2 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/color2");
Adafruit_MQTT_Subscribe pwm0 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/pwm0");*/

String LOCALIP="192.168.0.49";

const char COLOR_FEED[] = "domoticz/out";
Adafruit_MQTT_Publish color_topic = Adafruit_MQTT_Publish(&mqtt, COLOR_FEED);

int MQTT_ReconectCounter=5;

int rFlame = 255;
//int gFlame = 96;
//int bFlame = 12;
int gFlame = 56;
int bFlame = 2;
int colors[3] = {0, 0, 0};
bool updateColor = false;


int gamma1[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };   


void changeState()
{
  //digitalWrite(LED, !(digitalRead(LED)));  //Invert Current State of LED  
}

char* GetModeSValue(int index)
{
  switch (index)
  {
    case 0: {return "Simple"; break;}
    case 1: {return "All"; break;}
    case 2: {return "Rainbow"; break;}
    case 3: {return "Theater"; break;}
    case 4: {return "Flame"; break;}
    case 5: {return "NightMode"; break;}
    case 6: {return "Off"; break;}
    case 7: {return "Off Dual"; break;}
    case 8: {return "Off All"; break;}
    default: {return "none"; break;}
  }
}

int GetModeNValue(int idx)
{
  switch (idx)
  {
    case 0: {return MODE_SIMPLE; break;}
    case 1: {return MODE_ALL; break;}
    case 2: {return MODE_RAINBOW; break;}
    case 3: {return MODE_THEATER; break;}
    case 4: {return MODE_FLAME; break;}
    case 5: {return MODE_NIGHT; break;}
    case 6: {return MODE_OFF; break;}
    case 7: {return MODE_OFF_DUAL; break;}
    case 8: {return MODE_OFF_ALL; break;}
    default: {return MODE_OFF; break;}
  }
}


void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (WiFi.status() != WL_CONNECTED)
  {
    return;
  }
  if (mqtt.connected()) {
    return;
  }
  if (MQTT_ReconectCounter <= 0) {return;}
  Serial.print("Connecting to MQTT... ");
  for (int i = 0; i<3; i++) 
  {
    HTTP.handleClient();
    ret = mqtt.connect();
    if (ret == 0) 
    {
      Serial.println("MQTT connected");
      MQTT_ReconectCounter=5;
      return;
    }
    else
    { // connect will return 0 for connected
      switch (ret) 
      {
        case 1: Serial.println("Wrong protocol"); break;
        case 2: Serial.println("ID rejected"); break;
        case 3: Serial.println("Server unavailable"); break;
        case 4: Serial.println("Bad user/password"); break;
        case 5: Serial.println("Not authenticated"); break;
        case 6: Serial.println("Failed to subscribe"); break;
        default: Serial.print("Couldn't connect to server, code: ");
        Serial.println(ret);
        break;
      }
    }
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    HTTP.handleClient();
    MQTT_ReconectCounter--;
    delay(1000);// wait 5 seconds
  }  
}



void WakeUp()
{
  if (APMode) {return;}
  Serial.println("protocol: wakeup");
  SendWakeUp(MQTT_IDX);
}

void SendWakeUp(int idx)
{
  if (!mqtt.connected()) {
    return;
  }
  DynamicJsonBuffer JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["idx"] = idx;
  JSONencoder["wakeup"] = 0;
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);
  //mqttclient.publish("domoticz/out", JSONmessageBuffer);
  if (color_topic.publish(JSONmessageBuffer) == true) 
  {   
    //Serial.println("Success sending message");    
  } 
  else 
  {  
    //Serial.println("Error sending message");   
  }
}

void SendDomMess(int idx, char* nvalname, int nval, char* svalname, char* svalvalue)
{
  if (!mqtt.connected()) {
    return;
  }
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& JSONencoder = JSONbuffer.createObject();
  JSONencoder["idx"] = idx;
  JSONencoder[nvalname] = nval;
  JSONencoder[svalname] = svalvalue;
  JSONencoder["IP"] = myIP;
  JSONencoder["uptime"] = uptime();
  JSONencoder["freemem"] = ESP.getFreeHeap();
  Serial.println(ESP.getFreeHeap());
  char JSONmessageBuffer[100];
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  if (color_topic.publish(JSONmessageBuffer) == true) 
  {   
    //Serial.println("Success sending message");    
  } 
  else 
  {  
    //Serial.println("Error sending message");   
  }
}

String GetSecondZoneMode(int _SecondZoneMode)
{
  switch (_SecondZoneMode)
  {
    case 0: {return "Z2: Off"; break;}
    case 1: {return "Z2: On"; break;}
    default: {return "Z2: Off"; break;}
  }  
}

void changeMode(int c)
{
  if (c==7)
  {
    SecondZoneMode = false;
    saveConfig();                        // Функция сохранения данных во Flash  
    ModeOffSecondZone();
    SendDomMess(MQTT_IDX, "nvalue", MODE_OFF_DUAL, "svalue1", GetModeSValue(MODE_OFF_DUAL));
    //drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true);     
    drawDefault();
    return;
  }
  if (c==8)
  {
    SecondZoneMode = false;
    CurrentLedMode = 6;
    saveConfig();                        // Функция сохранения данных во Flash  
    ModeOffSecondZone();    
    updateColor = true;    
    needUpdate = true;
    setBreak = true;    
    SendDomMess(MQTT_IDX, "nvalue", MODE_OFF_ALL, "svalue1", GetModeSValue(MODE_OFF_ALL));    
    drawDefault();
    //drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true);     
    return;
  }
  
  updateColor = true;
  CurrentLedMode = c; 
  needUpdate = true;
  setBreak = true;
  //char* sval = GetModeSValue(CurrentLedMode);        
  SendDomMess(MQTT_IDX, "nvalue", GetModeNValue(CurrentLedMode), "svalue1", GetModeSValue(CurrentLedMode));
  //drawMode(sval, GetSecondZoneMode(SecondZoneMode), tempC, true);
  drawDefault();
}

void slidercallback(double x) {
  Serial.println(x);
  changeMode((int)x);
}

void SetRGB(int cRGB)
{
  r = (cRGB >> 16) & 0xFF;
  Serial.print("R: ");
  Serial.println(r);
  g = (cRGB >> 8) & 0xFF;
  Serial.print("G: ");
  Serial.println(g);
  b = cRGB & 0xFF;
  Serial.print("B: ");
  Serial.println(b);
}

void SetRGBSecondZone(int cRGB)
{
  r1 = (cRGB >> 16) & 0xFF;
  g1 = (cRGB >> 8) & 0xFF;
  b1 = cRGB & 0xFF;
}

void SetRGB2(int cRGB)
{
  r2 = (cRGB >> 16) & 0xFF;
  g2 = (cRGB >> 8) & 0xFF;
  b2 = cRGB & 0xFF;
}

void colorcallback(double x) {
  Serial.print("color is: ");
  Serial.println(x);
  SetRGB((int)x);
  changeMode(MODE_SIMPLE);
}

void colorcallback1(double x) {
  Serial.print("color is: ");
  Serial.println(x);
  SetRGBSecondZone((int)x);
  SecondZoneMode = true;
  saveConfig();                        // Функция сохранения данных во Flash  
  ModeSimpleSecondZone();
  drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true); 
}

void colorcallback2(double x) {
  Serial.print("color is: ");
  Serial.println(x);
  SetRGB2((int)x);
  saveConfig();                        
  //changeMode(MODE_SIMPLE);
}

void SetPWM(int _value)
{
  analogWrite(PWMPIN, _value);
}

void pwmcallback0(double x) {
/*  CurrentPWM = (int)x;
  SetPWM(CurrentPWM);*/
}

void allcallback(char* topic, byte* payload, unsigned int length)
{
  Serial.println("topic: ");
  Serial.println(topic);
  Serial.println("payload: ");
  //Serial.println(payload.toStr());  
}

void SubscribeTopics()
{  
  if (MQTT_IDX <= 0) {return;}
  
  //char* c_idx new char[]
  //(char*) m1 = String(MQTT_IDX, DEC).c_str();
  String Nstr = "/idx" + String(MQTT_IDX) + "/nvalue";  
  String Cstr = "/idx" + String(MQTT_IDX) + "/color";  
  String Cstr1 = "/idx" + String(MQTT_IDX) + "/color1";  
  String Cstr2 = "/idx" + String(MQTT_IDX) + "/color2";  
  String Cpwm0 = "/idx" + String(MQTT_IDX) + "/pwm0";  
  
  slider0 = Adafruit_MQTT_Subscribe(&mqtt, Str2Char(Nstr));
  color0 = Adafruit_MQTT_Subscribe(&mqtt, Str2Char(Cstr));
  color1 = Adafruit_MQTT_Subscribe(&mqtt, Str2Char(Cstr1));
  /*color2 = Adafruit_MQTT_Subscribe(&mqtt, Str2Char(Cstr2));
  pwm0 = Adafruit_MQTT_Subscribe(&mqtt, Str2Char(Cpwm0));*/
  /*slider0 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/nvalue");
  color0 = Adafruit_MQTT_Subscribe(&mqtt, "/idx29/color");*/
  slider0.setCallback(slidercallback); 
  color0.setCallback(colorcallback);  
  color1.setCallback(colorcallback1);  
  /*color2.setCallback(colorcallback2);  
  pwm0.setCallback(pwmcallback0);  */
  mqtt.subscribe(&slider0);
  mqtt.subscribe(&color0);  
  mqtt.subscribe(&color1);  
  /*mqtt.subscribe(&color2);  
  mqtt.subscribe(&pwm0);  */
  //mqtt.subscribe(&all0);  
}

void Strip_INIT()
{
  strip.begin();  
}

void Effects_INIT() {
  SubscribeTopics();
  //ESP.wdtEnable(WDTO_4S);
  RestoreSecondZone();
}

void ModeSimple()
{
  for(uint16_t i=0;i<EFFECTSPIXELS;i++)
    {
      //strip.setPixelColor(EFFECTSPIXELS - i, r, g, b); 
      strip.setPixelColor(i, r, g, b);
      //strip.show();
    }
    //strip.setPixelColor(0, r, g, b);
    strip.show();
}

void ModeSimpleSecondZone()
{
  int aI = ((UseStripAsActivityLed)?1:0);
  //drawMode(Str2Char(String(EFFECTSPIXELS) + ":" + String(TOTALPIXELS)), -100, true);
  for(uint16_t i=EFFECTSPIXELS;i<TOTALPIXELS-aI;i++)
    {
      strip.setPixelColor(i, r1, g1, b1); 
      //strip.setPixelColor(EFFECTSPIXELS + i, r1, g1, b1);
      strip.show();
    }
    //strip.setPixelColor(0, r1, g1, b1);
    strip.show();
}

void ModeOffSecondZone()
{
  int aI = ((UseStripAsActivityLed)?1:0);
  for(uint16_t i=EFFECTSPIXELS;i<TOTALPIXELS-aI;i++)
    {
      strip.setPixelColor(i, 0, 0, 0); 
      //strip.setPixelColor(EFFECTSPIXELS + i, r1, g1, b1);
      //strip.show();
    }
    //strip.setPixelColor(0, r1, g1, b1);
    strip.show();
}

void RestoreSecondZone()
{
  if (SecondZoneMode==1)
    {
      ModeSimpleSecondZone();
    }
}

void Effects_loop() {
  if (APMode) {return;}
  MQTT_connect();
  mqtt.processPackets(10);
  /*if(! mqtt.ping()) {
    mqtt.disconnect();
  }*/
   
  HTTP.handleClient();

  if ((CurrentLedMode==MODE_SIMPLE) && (needUpdate))
  {
    needUpdate = false;
    ModeSimple();
  }

  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_RAINBOW))
  {
    HTTP.handleClient();
    Serial.println("rainbowCycle");
    rainbowCycle(2);    
  }
  
  if (CurrentLedMode==MODE_ALL)
  {
    HTTP.handleClient();
    Serial.println("colorWipe");
    colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
    colorWipe(strip.Color(255, 0, 0), 50);  // Red
    colorWipe(strip.Color(0, 255, 0), 50);  // Green
    colorWipe(strip.Color(0, 0, 255), 50);  // Blue
  }

  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_THEATER))
  {
   HTTP.handleClient();
    Serial.println("theaterChase");
    theaterChase(strip.Color(127, 127, 127), 50); // White
    mqtt.processPackets(10);  
    theaterChase(strip.Color(127,   0,   0), 50); // Red
    mqtt.processPackets(10);  
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue
    mqtt.processPackets(10);  
  }

  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_RAINBOW))
  {
   HTTP.handleClient();
    Serial.println("rainbow");
    rainbow(20);
  }

  
  /*if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_RAINBOW))
  {
    HTTP.handleClient();
    Serial.println("theaterChaseRainbow");
    theaterChaseRainbow(50);
  }*/
  
  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_RAINBOW))
  {
    HTTP.handleClient();
    Serial.println("whiteOverRainbow");
    whiteOverRainbow(20,75,8);  
  }

  if (CurrentLedMode==MODE_ALL)
  {
    HTTP.handleClient();
    Serial.println("pulseWhite");
    pulseWhite(5); 
  }

  if (CurrentLedMode==MODE_ALL)
  {
    HTTP.handleClient();
    Serial.println("fullWhite");
    fullWhite();
    delay(100);
  }
  
  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_RAINBOW))
  {
    HTTP.handleClient();
    Serial.println("rainbowFade2White");
    rainbowFade2White(3,12,1);
  }
  
  if ((CurrentLedMode==MODE_ALL) || (CurrentLedMode==MODE_FLAME))
  {
   HTTP.handleClient();
    Serial.println("Flame");
    Flame();
  }

  if ((CurrentLedMode==MODE_OFF) && (needUpdate))
  { 
    needUpdate = false;
    for(uint16_t i=0; i<EFFECTSPIXELS; i++) 
    {
        HTTP.handleClient();
        delay(20);
        strip.setBrightness(255-(255/(EFFECTSPIXELS)*(i+1)));
        strip.show();
    }
    strip.setBrightness(0);
    strip.show();
    strip.setBrightness(255);  
     
    RestoreSecondZone();
  }

if ((CurrentLedMode==MODE_NIGHT) && (needUpdate))
  { 
    needUpdate = false;    
    //int EFFECTSPIXELS = 129;
    for(uint16_t i1=0; i1<EFFECTSPIXELS; i1++) 
    {
      for (uint16_t j=0; j<EFFECTSPIXELS; j++)
      {
        //strip.setPixelColor(EFFECTSPIXELS + i1, (255/(EFFECTSPIXELS))*(j+1),(80/(EFFECTSPIXELS))*(j+1),(10/(EFFECTSPIXELS))*(j+1) );
        strip.setPixelColor(i1, (255/(EFFECTSPIXELS))*(j+1),(80/(EFFECTSPIXELS))*(j+1),(10/(EFFECTSPIXELS))*(j+1) );
        //strip.setPixelColor((EFFECTSPIXELS + i1), strip.Color(250,80,10));
        //strip.setPixelColor((EFFECTSPIXELS - i1), strip.Color(250,80,10));
        
        
        HTTP.handleClient();
        //delay(5);
        if ((j%4)==0)
        {
          
          strip.setBrightness((TOTALPIXELS/EFFECTSPIXELS)*i1);
        }
        strip.show();
         
      }
    }
    strip.setBrightness(255);
    strip.show();
     
  }

}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void Flame()
{
  for (int y = 0; y<30; y++)
  {
    HTTP.handleClient();
    mqtt.processPackets(10);
     
    for(int x = 0; x <EFFECTSPIXELS; x++)
    {
      int flicker = random(0,40);
      int r1 = rFlame-flicker;
      int g1 = gFlame-flicker;
      int b1 = bFlame-flicker;
      if(g1<0) g1=0;
      if(r1<0) r1=0;
      if(b1<0) b1=0;
      strip.setPixelColor(x,r1,g1, b1);
     HTTP.handleClient();
    }
      if (setBreak)
      {
        setBreak= false;
        break;
      }
    strip.show();
    delay(random(50,150));
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<EFFECTSPIXELS; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    HTTP.handleClient();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    mqtt.processPackets(10);
     
    for(i=0; i<EFFECTSPIXELS; i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      HTTP.handleClient();
      if (setBreak)
      {
        setBreak= false;
        return;
      }
    }
      
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    mqtt.processPackets(10);  
    for(i=0; i< EFFECTSPIXELS; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / EFFECTSPIXELS) + j) & 255));
      HTTP.handleClient();
      if (setBreak)
      {
        setBreak= false;
        return;
      }
    }
    
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      mqtt.processPackets(10);  
      for (int i=0; i < (EFFECTSPIXELS - 2); i=i+2) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
        HTTP.handleClient();
        if (setBreak)
        {
          setBreak= false;
          return;
        }
      }
      
      strip.show();

      delay(wait);

      for (int i=0; i < (EFFECTSPIXELS - 2); i=i+2) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
        HTTP.handleClient();
        if (setBreak)
        {
          setBreak= false;
          return;
        }
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel    
    for (int q=0; q < 3; q++) {
      mqtt.processPackets(10);  
      for (int i=0; i < (EFFECTSPIXELS - 2); i=i+2) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        HTTP.handleClient();
        if (setBreak)
    {
      setBreak= false;
      return;
    }
      }
      strip.show();
      HTTP.handleClient();
      delay(wait);

      for (int i=0; i < (EFFECTSPIXELS - 2); i=i+2) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
        HTTP.handleClient();
        if (setBreak)
    {
      setBreak= false;
      return;
    }
      }
    }
    

  }
}

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
    mqtt.processPackets(10);  
      for(uint16_t i=0; i<EFFECTSPIXELS; i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, gamma1[j] ) );
          HTTP.handleClient();
          if (setBreak)
          {
            setBreak= false;
            return;
          }
        }
        
        delay(wait);
        strip.show();
      }

  for(int j = 255; j >= 0 ; j--){
    mqtt.processPackets(10);  
      for(uint16_t i=0; i<EFFECTSPIXELS; i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, gamma1[j] ) );
          HTTP.handleClient();
        }
        if (setBreak)
        {
          setBreak= false;
          return;
        }
        delay(wait);
        strip.show();
      }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
      mqtt.processPackets(10);  
      for(int i=0; i< EFFECTSPIXELS; i++) {

        wheelVal = Wheel(((i * 256 / EFFECTSPIXELS) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
        HTTP.handleClient();
        if (setBreak)
        {
          setBreak= false;
          return;
        }

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }
        if (setBreak)
        {
          setBreak= false;
          break;
        }

        strip.show();
        delay(wait);
    }
  
  }



  delay(50);


  for(int k = 0 ; k < whiteLoops ; k ++){

    for(int j = 0; j < 256 ; j++){
        mqtt.processPackets(10);   
        for(uint16_t i=0; i < EFFECTSPIXELS; i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, gamma1[j] ) );
            HTTP.handleClient();
          }
          if (setBreak)
          {
            setBreak= false;
            return;
          }
          strip.show();
        }

        delay(200);
        HTTP.handleClient();
    for(int j = 255; j >= 0 ; j--){

        for(uint16_t i=0; i < EFFECTSPIXELS; i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, gamma1[j] ) );
            HTTP.handleClient();
          }
          if (setBreak)
          {
            setBreak= false;
            return;
          }
        strip.show();
        }
  }

  delay(delayval);


}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  
  if(whiteLength >= EFFECTSPIXELS) whiteLength = EFFECTSPIXELS - 1;

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while(true){
    for(int j=0; j<256; j++) {
      mqtt.processPackets(10);  
      for(uint16_t i=0; i<EFFECTSPIXELS; i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
          HTTP.handleClient();
        }
        else{
          strip.setPixelColor(i, Wheel(((i * 256 / EFFECTSPIXELS) + j) & 255));
          HTTP.handleClient();
        }
        if (setBreak)
        {
          setBreak= false;
          return;
        }

      }

      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == EFFECTSPIXELS){
          loopNum++;
        }
        lastTime = millis();
      }

      if(loopNum == loops) return;
    
      head%=EFFECTSPIXELS;
      tail%=EFFECTSPIXELS;
        strip.show();
        delay(wait);
    }
  }
  
}
void fullWhite() {
  
    for(uint16_t i=0; i<EFFECTSPIXELS; i++) {
        strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        HTTP.handleClient();
    }
    strip.show();  
}


uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}
