
//for sensor
#include <DFRobot_PH.h>
#include <DFRobot_EC.h>
#include "DFRobot_ESP_EC.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "Adafruit_ADS1X15.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "EEPROM.h"

//for RTC
#include "Wire.h"
#include "RTClib.h"
 
//for store non-volatile data
#include <Preferences.h>

//for mqtt communication
#include <PubSubClient.h> //library for mqtt connection
#include <WiFi.h>
#include <ArduinoJson.h>

// Hardware Pins (DON'T CHANGE)
#define ec_pin 0 //pin A0 ADS1115
#define ph_pin 1 //pin A1 ADS1115
#define temp_pin 33 //
#define TRIG_PIN 25 // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 34 // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
#define waterPump_pin 32
#define DATA_INTERVAL 120000 //interval waktu pengiriman data scn
#define MAX_DUR_PUMP 10000 //maksimal nyala waktu pompa air
#define TIME_PUMP_ON 15 //waktu nyala sistem FLC Pompa (24 hours format 0 - 23)
 

#ifndef STRUCT_FILE
#include "structure.h"
#endif

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

//pinout for pump
#define a_pump_pin 13
#define b_pump_pin 12
#define water_pump_pin 32


//Gobal variable for tuning parameter



// Global parameter for MQTT Connection
WiFiClient espClient;
PubSubClient client(espClient);

// Global parameter for Sensor
DFRobot_ESP_PH_WITH_ADC ph;
DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;
OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);

//RTC
RTC_DS3231 rtc;

//Global parameter for preference
Preferences preferences;

//Global parameter for planting flag
unsigned int flagPlant; //0 untuk tidak dalam masa penanaman, 1: untuk masa penanaman
int flagPump = 0;
long duration_pump;

//Global parameter for FLC testing
unsigned int flagFLC = 0;
float set_ec = 2.1;
float set_water_level = 20;
int flag_ab_pump = 1;
int flag_water_pump = 1;

//Global parameter for mqtt subscription
String inString = "";    // variabel untuk menyimpan data string dari proses subscribe topic MQTT untuk diubah menjadi tipe data integer/float
unsigned long lastMsg = 0;
unsigned long lastMsg2 = 0;
unsigned long lastMsgPump = 0;
char msg[50];

//global parameter for water_level 
float empty_tank = 30;
float full_tank = 14.5;


// Global parameter for struct
Scn_parameter scn1;
FLC_parameter flc1;
TimeNow time1;

//Global parameter for WiFi & Mqtt Connection
const char* ssid = "WALT Greenhouse"; //username wifi
const char* password = "Greenhouse00"; //password wifi
const char* mqtt_server = "broker.hivemq.com"; //alamat broker mqtt, dalam hal ini saya menggunakan hivemq

void setup() {
  Serial.begin(115200);

  //Setup sensor
  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
  ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
  ads.setGain(GAIN_ONE);
  ph.begin();
  ads.begin();
  DS18B20.begin();
 
  //open preferences
  preferences.begin("FlagData", false);
  flagPlant = preferences.getUInt("flagPlant", 0); //read data flagPlant dari preferences, ini membuat data flagPlant sama seperti yang pengguna terakhir kali ubah
  flagFLC = preferences.getUInt("flagFLC", 0); //read data flagPlant dari preferences, ini membuat data flagPlant sama seperti yang pengguna terakhir kali ubah
  preferences.end();
  
  Serial.print("flagPlant: " + String(flagPlant));
  Serial.println(", flagFLC: " + String(flagFLC));

  //setup mqtt & wifi
  setup_wifi();
  setup_mqtt();

  //setup RTC
  setup_rtc();

  //pinout
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(a_pump_pin,OUTPUT);
  pinMode(b_pump_pin,OUTPUT);
  pinMode(water_pump_pin,OUTPUT);
  digitalWrite(water_pump_pin,HIGH);
  digitalWrite(a_pump_pin,LOW);
  digitalWrite(b_pump_pin,LOW);

  //fuzzy inizialitation
  fuzzification();
  fuzzy_rule();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
    reconnect_mqtt_broker();
  }
  client.loop();
  unsigned long now = millis();
  getNowTime();
  if((now - lastMsg2 > DATA_INTERVAL) && flagPlant == 1){
    lastMsg2 = now;
    send_to_mqtt();
//    getNowTime();
    Serial.println(returnDateTime() + "flagFLC: " + String(flagFLC) + " SCN || pH: " + String(scn1.ph) + ",ec: " + String(scn1.ec) + ",temp: " + String(scn1.water_temp) + ",water_level: " + String(scn1.water_level));
  }
  if((time1.jam == String(TIME_PUMP_ON)) && (flagFLC == 0) && (flagPlant == 1)){
    setupFLC();
    
  }
  if((time1.jam == add_1hour(String(TIME_PUMP_ON)))&& (flagFLC == 1)){
    resetFLC();
  }
  
  float voltage = ads.readADC_SingleEnded(0) / 10;
  float voltage2 = ads.readADC_SingleEnded(1) / 10;
  ec.calibration(voltage, read_temp()); // calibration process by Serail CMD
  ph.calibration(voltage2, read_temp());
}

void setup_wifi(){
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { //jika ESP32 gagal terkoneksi dengan WiFi
    delay(500);
    Serial.print(".");
  };

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt(){
  client.setServer(mqtt_server, 1883); //meng-set server/broker yang akan digunakan, dalam proyek ini digunakan hivemq dengan port 1883
  client.setCallback(callback); //call back merupakan fungsi yang akan mengambil data dari topic yang di-subscribe jika terdapat data yang di-publish dari broker  
  if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
    reconnect_mqtt_broker();
  }
  client.loop();
  client.publish("skripsi/iot_greenhouse/scn/setup", "SET UP ESP32");
}

void setup_rtc(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void callback(char* topic, byte* message, unsigned int length) { //fungsi untuk menerima data topic yang disubscribe jika terjadi proses publish dari broker menuju esp32

  char flagPumpStr[4];

  
  
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if(String(topic) == "skripsi/iot_greenhouse/scn/flagPlant"){
    inString = messageTemp;
    flagPlant = inString.toInt();
    setPrefFlagPlant();
  }
  else if(String(topic) == "skripsi/iot_greenhouse/scn/restart"){
    restartESP();
  }
  else if(String(topic) == "skripsi/iot_greenhouse/scn/manualPump"){
    manualPumpOn();
  }
//  else if(String(topic) == "skripsi/iot_greenhouse/flc/setting/trigger"){
//    inString = messageTemp;
//    flagFLC = inString.toInt();
//    if(flagFLC == 1){ //Mulai ambil dafa
//      settingFLC();  
//    } else if(flagFLC == 0){
//      resetFLC();
//    }
//    
//  }
//  else if(String(topic) == "skripsi/iot_greenhouse/startPump"){
//    if(flagFLC == 0){
//      startPump();
//    }
//  }
  
}

void reconnect_mqtt_broker(){
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
//    setup_wifi();
    // Attempt to connect
   String client_id = "ESP32Client";
    if (client.connect("clientId-zxFV4fGeOi")) { //jika esp32 sudah terkoneksi dengan broker, maka akan re-subscribe dengan daftar topic di bawah berikut
      Serial.println("connected");
      // daftar subscribe topic
      client.subscribe("skripsi/iot_greenhouse/scn/flagPlant");
      client.subscribe("skripsi/iot_greenhouse/scn/restart");
      client.subscribe("skripsi/iot_greenhouse/scn/manualPump");
//      client.subscribe("skripsi/iot_greenhouse/scn/manualPump");
    } else { //jika gagal menghubungkan dengan server
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void send_to_mqtt(){
  // ganti  
    Serial.println("sending data to mqtt") ;
    if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
      Serial.println("publishing to mqtt") ;
      reconnect_mqtt_broker();
      }
    client.loop();   
    sendSCNData(0,0);
}

void read_ph(){
  float voltage = ads.readADC_SingleEnded(ph_pin) / 10; // read the voltage
  scn1.ph = ph.readPH(voltage, read_temp()); // convert voltage to pH with temperature compensation
  
}

String add_1hour(String timeHour){
  if(timeHour == "23"){
    return String(0);
  } else{
    return String(timeHour.toInt()+1);
  }
}

void read_ec(){
  float voltage = ads.readADC_SingleEnded(ec_pin) / 10;
  scn1.ec = ec.readEC(voltage, read_temp()); // convert voltage to EC with temperature compensation
}

float read_temp(){
  DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
//  float temperatureC = 28;
  scn1.water_temp = temperatureC;
  return temperatureC;
}

void setupFLC(){
  //send setup command for FLC to db
  sendHistoryPompaData(0);
  flag_ab_pump = 0;
  flag_water_pump = 0;
  pump_on(flc1.ab_dur,flc1.wp_dur);
  flagFLC = 1;
  setPrefFlagFLC();  
}

void resetFLC(){
  flagFLC = 0;
  setPrefFlagFLC();
}

void getNowTime(){
  DateTime now = rtc.now();
  
  time1.jam = String(now.hour());
  time1.menit = String(now.minute());
  time1.detik = String(now.second());

}

String returnDateTime(){
  DateTime now = rtc.now();
  String date;

  date = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " (" + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()) + ") ";
  return date;
}

float read_waterLevel(){
  float water_level,duration_us, distance_cm;
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
  scn1.water_level = water_level;  
  return water_level;
}

void setPrefFlagPlant(){
  preferences.begin("FlagData", false);
  preferences.putUInt("flagPlant", flagPlant);
  preferences.end();  
  Serial.println("flagPlant saved to: " + String(flagPlant)); 
}



void setPrefFlagFLC(){
  preferences.begin("FlagData", false);
  preferences.putUInt("flagFLC", flagFLC);
  preferences.end();  
  Serial.println("flagFLC saved to: " + String(flagFLC)); 
}

void restartESP(){
  ESP.restart();
}

void manualPumpOn(){
  flag_ab_pump = 0;
  flag_water_pump = 0;
  sendHistoryPompaData(1);
//  fuzzy_kangkung = output_fuzzy_kangkung((scn1.ec- set_ec),scn1.water_level);
//  pump_on(5,10);
  pump_on(flc1.ab_dur,flc1.wp_dur);
  
}

void sendHistoryPompaData(int flc_mode){
  readAllSensor();
  
  Output_fuzzy fuzzy_kangkung;
  
  flc1.offset_ec = scn1.ec- set_ec;
  flc1.ec = scn1.ec;
  flc1.ph = scn1.ph;
  flc1.water_level = scn1.water_level;
  flc1.water_temp = scn1.water_temp;

  flc1.set_water_level = set_water_level;
  flc1.set_ec = set_ec;

  //Apply Fuzzy Rule to Input Data
  fuzzy_kangkung = output_fuzzy_kangkung(flc1.offset_ec,flc1.water_level);
//  fuzzy_kangkung = output_fuzzy_kangkung(-0.33,15);

  flc1.ab_dur = fuzzy_kangkung.output_ab_pump;
  flc1.wp_dur = fuzzy_kangkung.output_water_pump;
  flc1.flc_mode = flc_mode;

  
  StaticJsonDocument<256> doc;
  doc["offset_ec"] = flc1.offset_ec;
  doc["ph"] = flc1.ph;
  doc["ec"] = flc1.ec;
  doc["water_level"] = flc1.water_level;
  doc["water_temp"] = flc1.water_temp;
  doc["ab_dur"] = flc1.ab_dur;
  doc["wp_dur"] = flc1.wp_dur;
  doc["set_water_level"] = flc1.set_water_level;
  doc["set_ec"] = flc1.set_ec;
  doc["flc_mode"] = flc1.flc_mode;
  
  char out[256];

  int b = serializeJson(doc,out);


  Serial.println("sending data to mqtt") ;
  if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
    Serial.println("publishing to mqtt") ;
    reconnect_mqtt_broker();
    }
  client.loop(); 

  client.publish("skripsi/iot_greenhouse/flc/history/json", out);
}

void sendSCNData(int ab_pump,int water_pump){
  readAllSensor();

  StaticJsonDocument<256> doc;
  doc["water_temp"] = scn1.water_temp;
  doc["ph"] = scn1.ph;
  doc["ec"] = scn1.ec;
  doc["water_level"] = scn1.water_level;
  doc["ab_pump"] = ab_pump;
  doc["water_pump"] = water_pump;
  doc["flagFLC"] = flagFLC;
  
  char out[128];

  int b = serializeJson(doc,out);

  client.publish("skripsi/iot_greenhouse/scn/json", out);
}

void pump_on(float ab_dur,float wp_dur){
  Serial.println("tes1");
  unsigned long now = millis();
  unsigned long msg_ab = millis();
  unsigned long msg_wp = millis();
  
  sendSCNData(1,1);
  
  digitalWrite(a_pump_pin,HIGH);
  digitalWrite(b_pump_pin,HIGH);
  digitalWrite(water_pump_pin,LOW);
  
  while(!(flag_ab_pump && flag_water_pump)){
    msg_ab = millis();
    msg_wp = millis();
    
    if((msg_ab - now)/1000 > (ab_dur + 0.5)){
      digitalWrite(a_pump_pin,LOW);
      digitalWrite(b_pump_pin,LOW);
      if((msg_ab - lastMsgPump) > DATA_INTERVAL){
        lastMsgPump = msg_ab;
        sendSCNData(0,1);
      }
      flag_ab_pump = 1;
    }
    if((msg_wp - now)/1000 > (wp_dur - 5)){
      digitalWrite(water_pump_pin,HIGH);
      flag_water_pump = 1;
      
    }
  }
  sendSCNData(0,0);
}

void readAllSensor(){
  read_ph();
  read_ec();
  read_temp();
  read_waterLevel();
}

void startPump(){
  digitalWrite(a_pump_pin,HIGH);
  digitalWrite(b_pump_pin,HIGH);
  digitalWrite(water_pump_pin,LOW);
  delay(5000);
  digitalWrite(a_pump_pin,LOW);
  digitalWrite(b_pump_pin,LOW);
  digitalWrite(water_pump_pin,HIGH);
}
