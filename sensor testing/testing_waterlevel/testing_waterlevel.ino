/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-ultrasonic-sensor
 */

//#define TRIG_PIN 33 // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
//#define ECHO_PIN 34 // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin

//new
#define TRIG_PIN 25
#define ECHO_PIN 34


float empty_tank = 30;
float full_tank = 14.5;

float duration_us, distance_cm;

void setup() {
  // begin serial port
  Serial.begin (115200);

  // configure the trigger pin to output mode
  pinMode(TRIG_PIN, OUTPUT);
  // configure the echo pin to input mode
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
//  digitalWrite(TRIG_PIN, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(TRIG_PIN, LOW);
//
//  // measure duration of pulse from ECHO pin
//  duration_us = pulseIn(ECHO_PIN, HIGH);
//
//  // calculate the distance
//  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  ;
  Serial.print("water_level: ");
  Serial.print(read_waterLevel());
  Serial.println(" LITER");

  delay(500);
}

float read_waterLevel(){
  float water_level;
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  if(distance_cm > empty_tank){
    water_level = 0;
  }
  else{
    water_level = ((empty_tank - distance_cm)/(empty_tank - full_tank))*30; //in percent
  }
  return water_level;
}
