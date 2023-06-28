#include "Arduino.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_ADS1X15.h"
#include "EEPROM.h"

#define temp_pin 33

OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);
DFRobot_ESP_PH_WITH_ADC ph;
Adafruit_ADS1115 ads;

float voltage, phValue, temperature = 25;

//float readTemperature()
//{
//  //add your code here to get the temperature from your temperature sensor
//  if (isnan(event.temperature)) {
//    Serial.println(F("Error reading temperature!"));
//  }
//  else {
//    Serial.print(F("Temperature: "));
//    Serial.print(event.temperature);
//    Serial.println(F("°C"));
//  }
//  sensors.requestTemperatures();
//  return sensors.getTempCByIndex(0);
//}

void setup()
{
  Serial.begin(115200);

  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom

  ph.begin();
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
    /**
     * index 0 for adc's pin A0
     * index 1 for adc's pin A1
     * index 2 for adc's pin A2
     * index 3 for adc's pin A3
    */
    voltage = ads.readADC_SingleEnded(1) / 10; // read the voltage
    Serial.print("voltage:");
    Serial.println(voltage, 4);

    temperature = readTemperature(); // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");

    phValue = ph.readPH(voltage, 25); // convert voltage to pH with temperature compensation
    Serial.print("pH:");
    Serial.println(phValue, 4);
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
    DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
//  scn1.water_temp = temperatureC;
  return temperatureC;
}
