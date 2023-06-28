
#define bjt 25
#define bjt2 18


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(bjt,OUTPUT);
  pinMode(bjt2,OUTPUT);
  digitalWrite(bjt,LOW);
  digitalWrite(bjt2,LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    if(input == "on"){
      digitalWrite(bjt,HIGH);
      digitalWrite(bjt2,HIGH);
    } else if(input == "off"){
      digitalWrite(bjt,LOW);
      digitalWrite(bjt2,LOW);
    }
  }
}
