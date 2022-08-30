#include "Wire.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"


const float SEA_LEVEL_PRESSURE_HPA = 1013.25;
const int DELAY = 3000;
const int STARTUP_DELAY = 500;

Adafruit_BMP280 bme;

void BMP_init() 
{
 if(!bme.begin(0x76))
 {
   Serial.println("Could not find a valid BME280 sensor, check wiring!"); 
   BMP_enable = false;
 while (1)
 {
     yield();
     delay(DELAY);
   }
 }
 BMP_enable = true;
 delay(STARTUP_DELAY);
}

void BMP_loop() 
{
 if (!BMP_enable) {return;}
 tempC = bme.readTemperature();
// float humidity = bme.readHumidity();
 float pressurePascals = bme.readPressure();

// Print to serial monitor
 //printToSerial(tempC, 0, pressurePascals);

// Display data on screen in metric units
 //drawWithMetricUnits(tempC, 0, pressurePascals);
 drawDefault();
 /*yield();
  char* Svalue = new char[String(tempC).length()+1];
  strcpy(Svalue, String(tempC).c_str());
  Svalue[String(tempC).length()+1]='\0';*/
 SendDomMess(MQTT_IDX, "nvalue", tempC, "svalue", (char*)(String(tempC).c_str()));
 //delay(DELAY);
}
