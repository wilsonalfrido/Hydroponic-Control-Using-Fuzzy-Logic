#include <DFRobot_PH.h>
#include <DFRobot_EC.h>
#include "DFRobot_ESP_EC.h"
#include "DFRobot_ESP_PH_WITH_ADC.h"
#include "Adafruit_ADS1X15.h"
#include <PubSubClient.h> //library for mqtt connection
#include <WiFi.h>
#include <ArduinoJson.h>
#include "EEPROM.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Hardware Pins (DON'T CHANGE)
#define ec_pin 0 //pin A0 ADS1115
#define ph_pin 1 //pin A1 ADS1115
#define temp_pin 32



#define RPWM 25
#define LPWM 26
#define REN 33
#define LEN 32


typedef struct scn_value{
  float water_temp ;
  float temp;
  float ph ;
  float ec ;
  int ket; //keterangan, 0->START TESTING, 1; END TESTING
  int pengaduk; // 0-> motor dc nyala, 1-> motor dc mati
  
} Scn_value;



int bjt_sw;

WiFiClient espClient;
PubSubClient client(espClient);
Scn_value scn1;

DFRobot_ESP_PH_WITH_ADC ph;
DFRobot_ESP_EC ec;
Adafruit_ADS1115 ads;

OneWire pin_DS18B20(temp_pin);
DallasTemperature DS18B20(&pin_DS18B20);

float voltage, ecValue, temperature = 25;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);



  
  setup_wifi();
  setup_mqtt();

 
//  pinMode(RPWM,OUTPUT);
//  pinMode(LPWM,OUTPUT);
//  pinMode(LEN,OUTPUT);
//  pinMode(REN,OUTPUT);
//  digitalWrite(REN,HIGH);
//  digitalWrite(LEN,HIGH);
//
//  bjt_sw = 0;
  
  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
  ec.begin();//by default lib store calibration k since 10 change it by set ec.begin(30); to start from 30
  ads.setGain(GAIN_ONE);
  ph.begin();
  ads.begin();
  DS18B20.begin();
  //dummy scn value
  scn1.water_temp = 25;
//  scn1.ph = 7.97;
//  scn1.ec = 2.5;
  scn1.ket = 0;
//  scn1.pengaduk = 1;

}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long timepoint = millis();
  static unsigned long motordelay = millis();
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    if(input == "start"){
      scn1.ket = 0;
      Serial.println("ph,ec,water_temp,ket,pengaduk");
      while(Serial.readStringUntil('\n') != "end"){
        int pot=analogRead(35);
        int out1=map(pot,0,4095,0,120);
        analogWrite(LPWM,out1);
        analogWrite(RPWM,0);
        if(pot == 0){
          scn1.pengaduk = 0;
        } else {
          scn1.pengaduk = 1;
        }
        
        if(millis()-timepoint > 4000){
          timepoint = millis();
                      read_ph();
            read_ec();
            Serial.print(String(scn1.ph) + ",");
            Serial.print(String(scn1.ec) + ",");
            Serial.print(String(scn1.water_temp) + ",");
            Serial.print(String(scn1.ket) + ",");
            Serial.print(String(scn1.pengaduk) + ",");
            Serial.println();
            
            send_to_mqtt();
//          if(bjt_sw == 0){
//            digitalWrite(pin_bjt_sw_up,LOW); //ec sensor on
//            digitalWrite(pin_bjt_sw_down,LOW); //ec sensor on
//            read_ph();
//            read_ec();
//            bjt_sw = 1;
//          } else if (bjt_sw == 1){
//            digitalWrite(pin_bjt_sw_up,HIGH); //ec sensor on
//            digitalWrite(pin_bjt_sw_down,HIGH); //ec sensor off
//            read_ph();
////            read_ec();
//            Serial.print(String(scn1.ph) + ",");
//            Serial.print(String(scn1.ec) + ",");
//            Serial.print(String(scn1.water_temp) + ",");
//            if(scn1.ket == 0){
//               Serial.print("START,");
//            } else if(scn1.ket == 1){
//               Serial.print("MEASURE,");
//            } else{
//               Serial.print("END,");
//            }
//            (scn1.pengaduk == 0) ? Serial.print("OFF") : Serial.print("ON");
//            Serial.println();
//            send_to_mqtt();
//            bjt_sw = 0;
//            
//          }

        }
        if(Serial.readStringUntil('\n') == "mes"){
            scn1.ket = 1;
        }
      }
      scn1.ket = 2;
      send_to_mqtt();
      Serial.println("end");
    } else {
      Serial.println("invalid input");
    }
  }

}

void setup_mqtt(){
  const char* mqtt_server = "broker.hivemq.com";
  client.setServer(mqtt_server, 1883); //meng-set server/broker yang akan digunakan, dalam proyek ini digunakan hivemq dengan port 1883
  client.setCallback(callback); //call back merupakan fungsi yang akan mengambil data dari topic yang di-subscribe jika terdapat data yang di-publish dari broker  
}

void callback(char* topic, byte* message, unsigned int length) { //fungsi untuk menerima data topic yang disubscribe jika terjadi proses publish dari broker menuju esp32
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  StaticJsonDocument <256> doc; //src: http://www.steves-internet-guide.com/arduino-sending-receiving-json-mqtt/
  deserializeJson(doc,message);
//
//  params.ec_ctrl_min = doc["ecMin"];
//  params.ec_ctrl_max = doc["ecMax"];
//  params.ph_ctrl_min = doc["phMin"];
//  params.ph_ctrl_max = doc["phMax"];
  //set_params(doc["phMin"],doc["phMax"],doc["ecMin"],doc["ecMax"]);

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

void setup_wifi(){
  const char* ssid = "Ckkk"; //username wifi
  const char* password = "acong2308"; //password wifi
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { //jika ESP32 gagal terkoneksi dengan WiFi
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
      client.subscribe("skripsi/mqtt/scn") ;
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
  doc["ket"] = scn1.ket;
  doc["pengaduk"] = scn1.pengaduk;


  char out[128];

  int b = serializeJson(doc,out);

  client.publish("skripsi/iot_greenhouse/scn/json", out);
  
  char water_temp[5], ph[5], ec[5],ket[5],pengaduk[5];

  String(scn1.water_temp).toCharArray(water_temp, 5) ;
  String(scn1.ph).toCharArray(ph, 5) ;
  String(scn1.ec).toCharArray(ec, 5) ;
  String(scn1.ket).toCharArray(ket, 5) ;
  String(scn1.pengaduk).toCharArray(pengaduk, 5) ;
  client.publish("skripsi/iot_greenhouse/params/water_temp", water_temp);
  client.publish("skripsi/iot_greenhouse/params/pH", ph);
  client.publish("skripsi/iot_greenhouse/params/EC", ec);
  client.publish("skripsi/iot_greenhouse/params/ket", ket);
  client.publish("skripsi/iot_greenhouse/params/pengaduk", pengaduk);  

}


void testing(){
  
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
