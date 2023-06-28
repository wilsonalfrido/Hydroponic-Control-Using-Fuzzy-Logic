#include <OneWire.h>
#include <DallasTemperature.h>


#define temp_pin 35

OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);


float voltage, ecValue, temperature = 25;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DS18B20.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

    temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1);
    Serial.println("^C");
    delay(1000);


  
}

float readTemperature()
{
  DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
  return temperatureC;
}
