#include <SimpleTimer.h>    
#include <ESP8266WiFi.h>     
#include "Adafruit_MQTT.h"  
#include "Adafruit_MQTT_Client.h"  
#include <DHT.h>                   
#include <OneWire.h>             
#include <DallasTemperature.h>
#include "MQ135.h"
/* ===========================================*/
#define WLAN_SSID       "iPhone"
#define WLAN_PASS       "123456789"
/* ===========================================*/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "shtereff1"
#define AIO_KEY         "6b7cc0caf3b14609a2f4a88d3e3f1c1f"
/* ===========================================*/
WiFiClient client; 
/* ===========================================*/
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/* ===========================================*/
Adafruit_MQTT_Publish AirTempValueObj = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/air-temperature");
Adafruit_MQTT_Publish HumValueObj = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish CO2LevelValueObj = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/co2-level");
Adafruit_MQTT_Publish WaterTempValueObj = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water-temperature");
Adafruit_MQTT_Publish WaterLevelValueObj = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water-level");
/* ===========================================*/
Adafruit_MQTT_Subscribe AirTempLimitSliderObj = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/air-temperature-limit");
Adafruit_MQTT_Subscribe HumLimitSliderObj = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/humidity-limit");
Adafruit_MQTT_Subscribe CO2LimitSliderObj = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/co2-level-limit");
Adafruit_MQTT_Subscribe RBLigthSliderObj = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/red-blue-light");
Adafruit_MQTT_Subscribe MotorSpeedSliderObj = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/motor-speed");  
/* ===========================================*/
const int DHT11analog = D0;   
const int DHT11relay  = D3;  
#define DHTTYPE DHT11 
DHT dht(DHT11analog, DHTTYPE,11);
const int CO2analog   =  A0 ;    
const int CO2relay   =   D4 ;   
MQ135 gasSensor = MQ135(CO2analog);
const int WaterTempSensor = D1;  
OneWire oneWirePin(WaterTempSensor);   
DallasTemperature DS18b20(&oneWirePin);  
const int WaterLevelSensor =   D2; 
         
const int MotorDirection = D7;
const int MotorStep = D8;
const int WaterPumpRelay = D5;

const int Red_Blue_relay = D6;  

/* ===========================================*/
int AirTempLimit=28;          
int HumLimit=70 ;            
int CO2Limit=330;             
int WaterTempLimit=28;        
int RBLigthLimit=7;         
int MotorSpeedLimit=40 ;    
/* ===========================================*/
int AirTempValue=0;
int HumValue=0;
int CO2LevelValue=0;
int WaterTempValue =0;
int WaterLevelValue =0;
/* ===========================================*/
SimpleTimer timer;
SimpleTimer timer2;
int timer2ID;
/* ===========================================*/
void MQTT_connect();
void MySubscribe();
void DHT11_sensor();
void CO2_sensor();
void WTemp_sensor();
void WLevel_sensor(); 
void RBLight();
void MOTOR_stepper();
void MyPublish();
/* ======================================================================================*/
void setup() { 
  Serial.println("Hello! This is the Smart-Grow-Box system!");
  Serial.begin(115200);
  Serial.print("Connecting to ");      
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println(); 
  /* ===========================================*/
  pinMode(DHT11analog, INPUT);  
  pinMode(DHT11relay, OUTPUT);          
  dht.begin();
  pinMode(CO2analog,INPUT);
  pinMode(CO2relay,OUTPUT);           
  pinMode(WaterTempSensor,INPUT);
  DS18b20.begin();      
  pinMode(WaterLevelSensor,INPUT);    
  pinMode(Red_Blue_relay, OUTPUT);   
  pinMode(MotorStep, OUTPUT);       
  pinMode(MotorDirection, OUTPUT);  
  pinMode(WaterPumpRelay,OUTPUT); 
  /* ===========================================*/
  mqtt.subscribe(&RBLigthSliderObj);
  mqtt.subscribe(&MotorSpeedSliderObj);
  mqtt.subscribe(&AirTempLimitSliderObj);
  mqtt.subscribe(&HumLimitSliderObj);
  mqtt.subscribe(&CO2LimitSliderObj);
  /* ===========================================*/
  timer.setInterval(22000, MyPublish);
  timer2ID=timer2.setInterval(3000+576*MotorSpeedLimit, MOTOR_stepper);
  


}   

/* =====================================================================================*/
void loop() {
  MQTT_connect();  
  
  timer2.run(); 
    timer.run();
 
  MySubscribe();
  
  RBLight();
  DHT11_sensor();
  CO2_sensor();
  WTemp_sensor(); 
  WLevel_sensor(); 

}  
/* ======================================================================================*/
void MySubscribe(){
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    if (subscription == &AirTempLimitSliderObj) {
      AirTempLimit = atoi((char *)AirTempLimitSliderObj.lastread);  // convert to a number
      Serial.println("---------------------");  
      Serial.print("Subscribed AirTempLimit : ");   
      Serial.print(AirTempLimit); 
      Serial.println(" *C "); 
      delay(20);
    }
    if (subscription == &HumLimitSliderObj) {
      HumLimit = atoi((char *)HumLimitSliderObj.lastread); 
      Serial.println("---------------------");  
      Serial.print("Subscribed HumLimit : "); 
      Serial.print(HumLimit); 
      Serial.println(" % "); 
      delay(20);
    }
    if (subscription == &CO2LimitSliderObj) {
      CO2Limit = atoi((char *)CO2LimitSliderObj.lastread); 
      Serial.println("---------------------");  
      Serial.print("Subscribed CO2Limit : "); 
      Serial.print(CO2Limit); 
      Serial.println(" ppm ");  
      delay(20);
    }
    if (subscription == &RBLigthSliderObj) {
      if (strcmp((char *)RBLigthSliderObj.lastread, "ON") == 0) {
        RBLigthLimit=1;
      }
      if (strcmp((char *)RBLigthSliderObj.lastread, "OFF") == 0) {
        RBLigthLimit=0;
      }
     
      Serial.println("---------------------");  
      Serial.print("Subscribed RBLigthLimit : "); 
      Serial.print(RBLigthLimit); 
      Serial.println(" lux ");   
      delay(20);
    }
    if (subscription == &MotorSpeedSliderObj) {
      MotorSpeedLimit = atoi((char *)MotorSpeedSliderObj.lastread);  
      Serial.println("---------------------");  
      Serial.print("Subscribed MotorSpeedLimit : "); 
      Serial.print(MotorSpeedLimit);    
      Serial.println(" RPM ");    
      delay(20);    
    }
  }
}
/* ======================================================================================*/
void MyPublish(){
  if (AirTempValueObj.publish(AirTempValue)) {
    Serial.print(F("OK!"));    
  }
  if (HumValueObj.publish(HumValue)) {
    Serial.print(F("OK!"));   
  }
  if (CO2LevelValueObj.publish(CO2LevelValue)) {
    Serial.print(F("OK!"));  
  }
  if (WaterTempValueObj.publish(WaterTempValue)) {
    Serial.print(F("OK!"));     
  }
  if (WaterLevelValueObj.publish(WaterLevelValue)) {
    Serial.print(F("OK!"));     
  }
  
  Serial.println("---------------------");   
  Serial.println("Published: ");
  Serial.print("  AirTemp: ");
  Serial.print(AirTempValue);
  Serial.println("*C , ");
  Serial.print("  Hum: ");
  Serial.print(HumValue); 
  Serial.println("% , ");
  Serial.print("  CO2: ");
  Serial.print(CO2LevelValue);
  Serial.println("ppm , ");
  Serial.print("  WaterTemp: ");
  Serial.print(WaterTempValue);
  Serial.println("*C , ");
  Serial.print("  WaterLevel: ");
  Serial.print(WaterLevelValue);
  Serial.println("cm "); 
}
/* ======================================================================================*/
void DHT11_sensor(){ 
  
  HumValue = dht.readHumidity();                        
  AirTempValue = dht.readTemperature();    
               
  if (isnan(HumValue) || isnan(AirTempValue) ){      
    Serial.println("Failed to read from DHT sensor!");  
    return;                                             
  } 
  
  if ((AirTempValue >= AirTempLimit) || (HumValue >=  HumLimit) ) {  
    digitalWrite(DHT11relay, LOW );                          
  }
  else {
    digitalWrite(DHT11relay, HIGH);     
  }  
} 
/* ======================================================================================*/
void CO2_sensor(){  
  CO2LevelValue = gasSensor.getRZero();
  if(CO2LevelValue>CO2Limit){           
    digitalWrite(CO2relay,HIGH);    
  }
  else{                         
    digitalWrite(CO2relay,LOW);   
  }    
}
/* ======================================================================================*/
void WTemp_sensor(){   

  DS18b20.requestTemperatures(); 
  WaterTempValue  = DS18b20.getTempCByIndex(0); 
  
//  if(WaterTempValue >= WaterTempLimit ){
//    digitalWrite(WaterTempRelay, HIGH); 
//  }
//  else {    
//    digitalWrite(WaterTempRelay, LOW);  
//  }
}

/* ======================================================================================*/
void WLevel_sensor() 
{
   WaterLevelValue = digitalRead(WaterLevelSensor); 
}

/* ======================================================================================*/
void RBLight(){
  if(RBLigthLimit==1){
    digitalWrite(Red_Blue_relay, LOW); 
  }
  else{
    digitalWrite(Red_Blue_relay, HIGH); 
  }
}
/* ======================================================================================*/

void MOTOR_stepper(){   
  int interv=3000+576*MotorSpeedLimit;
  timer2.deleteTimer(timer2ID);
  timer2ID = timer2.setInterval(interv,  MOTOR_stepper); 
  Serial.print("INTERVAL: ");
  Serial.println(interv); 
   
  int cicleUp;
  
  digitalWrite(WaterPumpRelay,HIGH);
  Serial.println("Water Pump is OFF: ");          
  Serial.println("---------------------");     
  
  digitalWrite(MotorStep,HIGH);
  Serial.print("Motor is ON : ");         
  Serial.print(MotorSpeedLimit);
  Serial.println("RPM");
  Serial.println("---------------------"); 
  
 

  for(cicleUp = 0; cicleUp < 288; cicleUp++){
    digitalWrite(MotorDirection,HIGH);
    delay(MotorSpeedLimit);   
    digitalWrite(MotorDirection,LOW);
    delay(MotorSpeedLimit);    
  }
   
   digitalWrite(MotorStep,LOW);
  Serial.println("Stepper Motor is  OFF ");          
  Serial.println("---------------------"); 
  
  digitalWrite(WaterPumpRelay,LOW);   
  Serial.println("Water Pump is ON : ");          
  Serial.println("---------------------");  
  
}

/* ======================================================================================*/
void MQTT_connect() {
  
  int8_t ret;
 
  if (mqtt.connected()) {
    return;// Stop if already connected.
  }
  
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
/* ======================================================================================*/
