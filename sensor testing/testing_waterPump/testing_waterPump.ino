void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(32,OUTPUT);

}

void loop() {
//  // put your main code here, to run repeatedly:
  digitalWrite(32,HIGH);
  delay(2000);
  digitalWrite(32,LOW);;
  delay(2000);

}
