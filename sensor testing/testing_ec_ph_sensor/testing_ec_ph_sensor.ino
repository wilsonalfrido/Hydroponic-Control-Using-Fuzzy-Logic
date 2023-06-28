#include "Arduino.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_ADS1X15.h"
#include "EEPROM.h"
#include "DFRobot_ESP_EC.h"

#define temp_pin 33

OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);
DFRobot_ESP_PH_WITH_ADC ph;
DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

float voltage, voltage2, phValue, ecValue, temperature = 25;


void setup()

{
  Serial.begin(115200);

  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom

  ph.begin();
  ec.begin();
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
    voltage2 = ads.readADC_SingleEnded(0) / 10;
    
//    Serial.print("voltage:");
//    Serial.println(voltage, 4);

    temperature = readTemperature(); // read your temperature sensor to execute temperature compensation
//    Serial.print("temperature:");
//    Serial.print(temperature, 1);
//    Serial.println("^C");

    phValue = ph.readPH(voltage, temperature); // convert voltage to pH with temperature compensation
    ecValue = ec.readEC(voltage2, temperature);
//    Serial.print("pH:");
//    Serial.println(phValue, 4);

    Serial.println("pH: " + String(phValue) + ",ec: " + String(ecValue) + ",temp: " + String(temperature));
  }
  ph.calibration(voltage, temperature); // calibration process by Serail CMD
  ec.calibration(voltage, temperature);
}

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
    DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
//  scn1.water_temp = temperatureC;
  return temperatureC;
}
