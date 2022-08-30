#include "Wire.h"
#include "SSD1306.h"

SSD1306 display(0x3c, PIN_SDA, PIN_SCL);

void InitSSD1306()
{
  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(100000); 
  display.init();
  //display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_16);
}

/*void drawWithMetricUnits(float tempC, float humidity, float pressurePascals)
{
 float pressureHectoPascals = pressurePascals / 100.0;
 display.clear();
 //display.drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT/2);
 display.drawString(20, 40, String(tempC) + " *C");
 drawMode(GetModeSValue(CurrentLedMode), false);
 display.display();
}*/

void drawLogo()
{
  int StartY = 8;
  int StartX = 46;
  //display.clear(); 
  for (int i=0; i< 10; i++)
  {
    if (i<5)
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 31 + i);
    }
    else
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 35);
    }
  }
  StartX += 24;
  for (int i=0; i< 10; i++)
  {
    if (i>5)
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 41 - i);
    }
    else
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 35);
    }
  }
  display.fillRect(StartX-10, StartY+26, 10, 10);
  display.display();
}

void drawSmallLogo()
{
  int StartY = 4;
  int StartX = 4;
  //display.clear(); 
  for (int i=0; i< 6; i++)
  {
    if (i<3)
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 15 + i);
    }
    else
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 18);
    }
  }
  StartX += 12;
  for (int i=0; i< 6; i++)
  {
    if (i>=3)
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 20 - i);
    }
    else
    {
      display.drawLine(StartX + i, StartY, StartX + i, StartY + 18);
    }
  }
  display.fillRect(StartX-4, StartY+14, 5, 5);  
}

void drawDefault()
{
  drawMode(GetModeSValue(CurrentLedMode), GetSecondZoneMode(SecondZoneMode), tempC, true);
}

void drawMode(String cMode, String SecondMode, float tempC, boolean clear)
{
 String tCpwm = ""; 
 if (clear) { 
   display.clear(); 
 }
 drawSmallLogo();
 //display.fillRect(0, 0, 128, 38);
 display.setTextAlignment(TEXT_ALIGN_CENTER);
 display.setFont(ArialMT_Plain_24);
 display.drawString(64, 21, cMode);
 /*display.setTextAlignment(TEXT_ALIGN_CENTER);
 display.setFont(ArialMT_Plain_10);
 display.drawString(64, 43, SecondMode);*/
 if (tempC > -100.0)
 {
   display.setTextAlignment(TEXT_ALIGN_CENTER);
   display.setFont(ArialMT_Plain_24);
   display.drawString(74, 0, String(tempC) + "°C");
 }
 display.setTextAlignment(TEXT_ALIGN_CENTER);
 display.setFont(ArialMT_Plain_16);
 //display.drawString(64, 54, String(myIP + " id: " + String(MQTT_IDX)));
 //display.drawString(64, 52, String("rpm: "+ String(CurrentPWM) + " id: " + String(MQTT_IDX)));
 if ((CurrentPWM==1500) || (CurrentPWM==0))
 {
    tCpwm = "Cooler: Off";
 }
 else
 {
    tCpwm = "Rpm: " + String(CurrentPWM);
 }
 display.drawString(64, 46, String(tCpwm + " " + String(SecondMode)));
 display.display();
}
