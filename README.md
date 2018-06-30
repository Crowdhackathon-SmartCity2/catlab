# catlab

# Περιγραφή : 
Κώδικας για την λειτουργία και διαχείριση ενός αυτοματοποιημένου συστήματος καλλιέργιας φυτών με την τεχνολογία της Αεροπονικής
# Τεχνολογία : 
Χρησιμοποιούνται οι αισθητήρες μικροκλιματισμού DHT11 , MQ135, DS18b20,Adafruit MQTT WiFi Access Point,Stepper Motor "NEMA 17" Speed and Water Pump Control 
# Βιβλιοθήκες : 
  #include <SimpleTimer.h>    
  #include <ESP8266WiFi.h>     
  #include "Adafruit_MQTT.h"  
  #include "Adafruit_MQTT_Client.h"  
  #include <DHT.h>                   
  #include <OneWire.h>             
  #include <DallasTemperature.h>
  #include "MQ135.h"
# Λειτουργίες : 
Εξαγωγή χαρακτηριστικών περιβάλλοντος απο τους αισθητήρες και αποστολεί τους στην ηλεκτρονική πλατφόρμα ADAFRUIT.
# Σχετ. Reference links 
        1)Adafruit MQTT WiFi Access Point 
            https://io.adafruit.com/shtereff1/profile
        2) DHT11 Digital Temperature and Humidity Sensor -DHT11:PRODUCT ID: 386- https://www.adafruit.com/product/386
            https://www.hellasdigital.gr/electronics/sensors/temperature-sensors/dht11-digital-temperature-and-humidity-sensor/
        3) MQ135- Carbon Dioxide Meter 
            https://www.hellasdigital.gr/electronics/sensors/gas-sensors/mq135-mq-135-air-quality-sensor-hazardous-gas-detection-module-for-arduino/
        4) DS18b20 Water Temperature Sensor 
            https://www.hellasdigital.gr/electronics/sensors/temperature-sensors/waterproof-temparature-sensor-ds18b20-1m-for-arduino/
        5) Analog TDS Sensor/Meter for Arduino(SEN0244)
            https://www.hellasdigital.gr/electronics/sensors/humidity-sensor/gravity-analog-tds-sensor-meter-for-arduino-sen0244
        6) Gravity: Analog pH Sensor / Meter Kit For Arduino (SEN0161) 
            https://www.hellasdigital.gr/electronics/sensors/humidity-sensor/gravity-analog-ph-sensor-meter-kit-for-arduino-sen0161
        7) 5050 Grow LED Flexible Strip - Red -Blue Brightness and Light Control
            https://grobotronics.com/led-strip-5050-14.4w-m-ip20-rgb.html
        8) Stepper Motor "NEMA 17" Speed and Water Pump Control 
            https://grobotronics.com/stepper-motor-42byghw208-2.6kg.cm.html
            Stepper Motor Driver(a4988) https://www.hellasdigital.gr/electronics/motors-and-drivers/drivers/a4988-stepstick-stepper-driver-for-rerrap-pololu-3d-printers%20/
        9) Liquid Pump Motor - Micro 12V
            https://grobotronics.com/liquid-pump-motor-micro-12v.html 
            
#username και password 
        kamyshtereva : c9df3eb4


