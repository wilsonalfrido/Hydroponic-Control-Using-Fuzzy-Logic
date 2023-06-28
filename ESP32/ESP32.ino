#ifndef FUZZY_LIB
#define FUZZY_LIB 1
#include <Fuzzy.h>
#endif 

#ifndef FUZZY_SET
#include "fuzzy_set_kangkung.h"
#endif

#ifndef FUZZIFICATION
#include "fuzzification.h"
#endif

#define a_pump_pin 13
#define b_pump_pin 12
#define water_pump_pin 32

int flag_delay_pump = 0;
int flag_ab_pump = 0;
int flag_water_pump = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//
  pinMode(a_pump_pin,OUTPUT);
  pinMode(b_pump_pin,OUTPUT);
  pinMode(water_pump_pin,OUTPUT);
  digitalWrite(water_pump_pin,HIGH);
  fuzzification();
  fuzzy_rule();

}

void loop() {
  // put your main code here, to run repeatedly:
  Output_fuzzy fuzzy_kangkung;
  
  float input_ec = -0.31;
  float input_water_level = 13.5;
  Serial.print("Input_ec : " + String(input_ec) + ", Input_wlevel : " + String(input_water_level));
  Serial.println();
  
//  fuzzy->setInput(1,input_ec);
//  fuzzy->setInput(2,input_ph);
//  fuzzy->fuzzify();
  fuzzy_kangkung = output_fuzzy_kangkung(input_ec,input_water_level);

//  float output_ph_up = fuzzy->defuzzify(1);
//  float output_ph_down = fuzzy->defuzzify(2);
//  float output_ab_pump = fuzzy->defuzzify(3);
  Serial.println("ab pump : " + String(fuzzy_kangkung.output_ab_pump) + " s");
  Serial.println("waterpump pump : " + String(fuzzy_kangkung.output_water_pump) + " s");

    if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    while(input == "cal"){
      digitalWrite(a_pump_pin,HIGH);
      digitalWrite(b_pump_pin,HIGH);
      digitalWrite(water_pump_pin,LOW);
      delay(5000);
            digitalWrite(a_pump_pin,LOW);
      digitalWrite(b_pump_pin,LOW);
      digitalWrite(water_pump_pin,HIGH);
      break;
    }
    while(input == "start"){  
      pump_on(fuzzy_kangkung.output_ab_pump,fuzzy_kangkung.output_water_pump);
      break;      

  }


}
  
  
}

void pump_on(float ab_dur,float wp_dur){
  Serial.println("tes1");
  unsigned long msg_ab = millis();
  unsigned long msg_wp = millis();
  unsigned long now = millis();
  digitalWrite(a_pump_pin,HIGH);
  digitalWrite(b_pump_pin,HIGH);
  digitalWrite(water_pump_pin,LOW);

  while(!(flag_ab_pump && flag_water_pump)){
    msg_ab = millis();
    msg_wp = millis();

    if((msg_ab - now)/1000 > ab_dur){
      digitalWrite(a_pump_pin,LOW);
      digitalWrite(b_pump_pin,LOW);
      flag_ab_pump = 1;
    }
    if((msg_wp - now)/1000> wp_dur){
      digitalWrite(water_pump_pin,HIGH);
      flag_water_pump = 1;
    }
  }
  flag_ab_pump = 0;
  flag_water_pump = 0;
}
