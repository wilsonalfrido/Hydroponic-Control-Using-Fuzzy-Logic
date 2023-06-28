#define pump_1 13
#define pump_2 12
#define pump_3 27
#define pump_4 26




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pump_1,OUTPUT);
  pinMode(pump_2,OUTPUT);
  pinMode(pump_3,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  

  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    while(input == "start"){
      static unsigned long timepoint = millis();
      Serial.println("START");
      digitalWrite(pump_1,HIGH);
      while(1){
        if(millis()-timepoint > 7000){
        digitalWrite(pump_1,LOW);
        input = "end";
        ESP.restart();
        break;
      }
        
      }
        
      

  }


}

}
