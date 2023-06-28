/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 32;     

// Setup a oneWire instance to communicate with any OneWire devices
//OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
//DallasTemperature sensors(&oneWire);

OneWire pin_DS18B20(oneWireBus);
DallasTemperature DS18B20(&pin_DS18B20);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  DS18B20.begin();
//  sensors.begin();
}

void loop() {
//  sensors.requestTemperatures(); 
  DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
  float temperatureF = DS18B20.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(2000);
}
