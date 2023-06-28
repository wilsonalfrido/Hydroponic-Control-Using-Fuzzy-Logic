//for sensor
#include <DFRobot_PH.h>
#include <DFRobot_EC.h>
#include "DFRobot_ESP_EC.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "Adafruit_ADS1X15.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "EEPROM.h"

//for store non-volatile data
#include <Preferences.h>

//for mqtt communication
#include <PubSubClient.h> //library for mqtt connection
#include <WiFi.h>
#include <ArduinoJson.h>

// Hardware Pins (DON'T CHANGE)
#define ec_pin 0 //pin A0 ADS1115
#define ph_pin 1 //pin A1 ADS1115
#define temp_pin 35 //
#define TRIG_PIN 25 // ESP32 pin GIOP23 connected to Ultrasonic Sensor's TRIG pin
#define ECHO_PIN 34 // ESP32 pin GIOP22 connected to Ultrasonic Sensor's ECHO pin
#define waterPump_pin 32
#define DATA_INTERVAL 4000 //interval waktu pengiriman data scn
#define MAX_DUR_PUMP 10000 //maksimal nyala waktu pompa air


//Gobal variable for tuning parameter



typedef struct scn_value{
  float water_temp ;
  float ph ;
  float ec ;
  float water_level;
  int waterPump;  
  int duration;
} Scn_value;

// Global parameter for MQTT Connection
WiFiClient espClient;
PubSubClient client(espClient);

// Global parameter for Sensor
DFRobot_ESP_PH_WITH_ADC ph;
DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;
OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);

//Global parameter for preference
Preferences preferences;

//Global parameter for planting flag
unsigned int flagPlant; //0 untuk tidak dalam masa penanaman, 1: untuk masa penanaman
int flagPump = 0;
long duration_pump;


//Global parameter for mqtt subscription
String inString = "";    // variabel untuk menyimpan data string dari proses subscribe topic MQTT untuk diubah menjadi tipe data integer/float
unsigned long lastMsg = 0;
char msg[50];

//global parameter for water_level 
float empty_tank = 30;
float full_tank = 14.5;


// Global parameter for struct
Scn_value scn1;

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
  preferences.begin("FlagPlant", false);
  flagPlant = preferences.getUInt("flagPlant", 1); //read data flagPlant dari preferences, ini membuat data flagPlant sama seperti yang pengguna terakhir kali ubah
  preferences.end();
  Serial.println("Kondisi flagPlant sebelumnya: " + String(flagPlant));

  //setup mqtt & wifi
  setup_wifi();
  setup_mqtt();

  //pinout
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(waterPump_pin,OUTPUT);

  digitalWrite(waterPump_pin,HIGH);

  scn1.waterPump = 0;
  scn1.duration = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
    reconnect_mqtt_broker();
  }
  client.loop();
  unsigned long now = millis();
  if ((now - lastMsg > DATA_INTERVAL) && flagPlant == 1) { //publish data suhu dan kelembapan tiap 2 detik 
    lastMsg = now;
    read_ph();
    read_ec();
    read_temp();
    read_waterLevel();
    Serial.println("pH: " + String(scn1.ph) + ",ec: " + String(scn1.ec) + ",temp: " + String(scn1.water_temp) + ",water_level: " + String(scn1.water_level));
    send_to_mqtt();
  }
//  float voltage = ads.readADC_SingleEnded(0) / 10;
//  float voltage2 = ads.readADC_SingleEnded(1) / 10;
//  ec.calibration(voltage, read_temp()); // calibration process by Serail CMD
//  ph.calibration(voltage2, read_temp());
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
    setFlagPlant() ;
  }
  else if(String(topic) == "skripsi/iot_greenhouse/akt/pompa/trigger"){
    inString = messageTemp;
    flagPump = inString.toInt();
    if(flagPump == 1){
      controlWaterPump();
      
    }
    
  }
  
  StaticJsonDocument <256> doc; //src: http://www.steves-internet-guide.com/arduino-sending-receiving-json-mqtt/
  deserializeJson(doc,message);
//
//  params.ec_ctrl_min = doc["ecMin"];
//  params.ec_ctrl_max = doc["ecMax"];
//  params.ph_ctrl_min = doc["phMin"];
//  params.ph_ctrl_max = doc["phMax"];
  //set_params(doc["phMin"],doc["phMax"],doc["ecMin"],doc["ecMax"]);

}

void reconnect_mqtt_broker(){
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
//    setup_wifi();
    // Attempt to connect
   String client_id = "ESP32Client";
    if (client.connect("clientId-aj0oR3fgv1")) { //jika esp32 sudah terkoneksi dengan broker, maka akan re-subscribe dengan daftar topic di bawah berikut
      Serial.println("connected");
      // daftar subscribe topic
      client.subscribe("skripsi/iot_greenhouse/scn/flagPlant");
      client.subscribe("skripsi/iot_greenhouse/akt/pompa/trigger");
    } else { //jika gagal menghubungkan dengan server
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void publish_param(){
  StaticJsonDocument<256> doc;
  doc["water_temp"] = scn1.water_temp;
  doc["ph"] = scn1.ph;
  doc["ec"] = scn1.ec;
  doc["water_level"] = scn1.water_level;
  doc["waterPump"] = scn1.waterPump;
  doc["duration"] = scn1.duration;

  char out[128];

  int b = serializeJson(doc,out);

  client.publish("skripsi/iot_greenhouse/scn/json", out);
  
//  char water_temp[5], ph[5], ec[5],ket[5],pengaduk[5];

//  String(scn1.water_temp).toCharArray(water_temp, 5) ;
//  String(scn1.ph).toCharArray(ph, 5) ;
//  String(scn1.ec).toCharArray(ec, 5) ;
//  String(scn1.ket).toCharArray(ket, 5) ;
//  String(scn1.pengaduk).toCharArray(pengaduk, 5) ;
//  client.publish("skripsi/iot_greenhouse/params/water_temp", water_temp);
//  client.publish("skripsi/iot_greenhouse/params/pH", ph);
//  client.publish("skripsi/iot_greenhouse/params/EC", ec);
//  client.publish("skripsi/iot_greenhouse/params/ket", ket);
//  client.publish("skripsi/iot_greenhouse/params/pengaduk", pengaduk);  

}

void send_to_mqtt(){
  // ganti  
    Serial.println("sending data to mqtt") ;
    if (!client.connected()) { //jika esp32 tidak terhubung dengan broker mosquitto, maka akan coba di reconnect
      Serial.println("publishing to mqtt") ;
      reconnect_mqtt_broker();
      }
    client.loop();   
    publish_param();
}

void read_ph(){
  float voltage = ads.readADC_SingleEnded(ph_pin) / 10; // read the voltage
  scn1.ph = ph.readPH(voltage, read_temp()); // convert voltage to pH with temperature compensation
  
}

void read_ec(){
  float voltage = ads.readADC_SingleEnded(ec_pin) / 10;
  scn1.ec = ec.readEC(voltage, read_temp()); // convert voltage to EC with temperature compensation
}

float read_temp(){
  DS18B20.requestTemperatures();  //Get temp value
  float temperatureC = DS18B20.getTempCByIndex(0);
  scn1.water_temp = temperatureC;
  return temperatureC;
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

void controlWaterPump(){
  unsigned long startTime = millis();
  unsigned long duration = 0;
  char flagPumpStr[4];
//  float water_level = read_waterLevel();
  while((duration < MAX_DUR_PUMP) && (read_waterLevel() <= 30)){
    duration = millis() - startTime; 
    digitalWrite(waterPump_pin,LOW);
  }
  duration_pump = (millis()- startTime)/1000;
  digitalWrite(waterPump_pin,HIGH);
  flagPump = 0;
  scn1.duration = duration_pump;
  scn1.waterPump = 1;
  send_to_mqtt();
  Serial.println("Pump Duration On: " + String(duration_pump) + " s");
  String(flagPump).toCharArray(flagPumpStr, 4);
  client.publish("skripsi/iot_greenhouse/akt/pompa/feedback",flagPumpStr);
  resetPump();
}

void resetPump(){
  scn1.duration = 0;
  scn1.waterPump = 0;
  digitalWrite(waterPump_pin,HIGH);
}

void setFlagPlant(){
  preferences.begin("FlagPlant", false);
  preferences.putUInt("flagPlant", flagPlant);
  preferences.end();  
  Serial.println("flagPlant saved to: " + String(flagPlant)); 
}

void restartESP(){
  ESP.restart();
}
