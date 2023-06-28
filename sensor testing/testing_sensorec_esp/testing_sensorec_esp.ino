
#include <OneWire.h>
#include <DallasTemperature.h>

#include "Arduino.h"
#include "Adafruit_ADS1X15.h"
#include "DFRobot_ESP_EC.h"
#include "EEPROM.h"



//library for water temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

#define temp_pin 32


DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;
OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);

 
-

void setup()
{

  
  Serial.begin(115200);
  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
  ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30

  DS18B20.begin();
  
  ads.setGain(GAIN_ONE);
  ads.begin();
}

void loop()
{
  static unsigned long timepoint = millis();

  
  if (millis() - timepoint > 1000U) //time interval: 1s
  {

    timepoint = millis();
    voltage = ads.readADC_SingleEnded(0) / 10;
    Serial.print("voltage:");
    Serial.println(voltage, 4);

//    temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
//    Serial.print("temperature:");
//    Serial.print(temperature, 1);
//    Serial.println("^C");

    ecValue = ec.readEC(voltage, 25); // convert voltage to EC with temperature compensation
    Serial.print("EC:");
    Serial.print(ecValue, 3);
    Serial.println("ms/cm");
  }
  ec.calibration(voltage, 25); // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
    DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
//  scn1.water_temp = temperatureC;
  return temperatureC;
}
