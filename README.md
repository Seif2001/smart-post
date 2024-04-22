# smart-post

A Smart Lamppost System (SLS) using ESP32 microcontrollers and the Cooja simulator.

## Components

1. **ESP32 DevKitC v4 Pinout**
   ![ESP32 DevKitC v4 Pinout](https://docs.espressif.com/projects/esp-idf/en/v5.1/esp32/_images/esp32-devkitC-v4-pinout.png)
   The datasheet can be viewed [here](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

2. **MQ135 Gas sensor**
   
   Measures gas concentration levels in parts per million (ppm), providing data on air quality
    
   The datasheet can be viewed [here](https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf)
   
4. **LDR Sensor**
   
   Measures Light intensity
   
   The datasheet can be viewed [here](https://components101.com/sites/default/files/component_datasheet/LDR%20Datasheet.pdf)
6. **DHT11 Sensor**
   
   Measures Tempreture and humidity levels
   
   The datasheet can be viewed [here](https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf)

   ## Libraries used in Aruino IDE
   ### Client

   1. **Adafruit Unified Sensor by Adafruit**
      Dependency for DHT sensor library
   2. **DHT sensor Library by AdaFruit**
      Responsible for interfacing the DHT sensor
   3. **Firebase Arduino Client Library for ESP8266 and ESP32 by Mobzit**
      Responsible for connecting and sending data to the fire base server
      
    ### Server
   1. **Adafruit Unified Sensor by Adafruit**
      Dependency for DHT sensor library
   2. **DHT sensor Library by AdaFruit**
      Responsible for interfacing the DHT sensor

   ## Steps to Run
   ### Server First
   1. Connect circuit and connect to computer via thunderbolt cable
   2. Install server libraries
   3. Run code
   ### Then Client
   1. Connect circuit and connect to computer via thunderbolt cable
   2. Install server libraries
   3. Open hotspot and type your ssid and password in WIFI_SSID and WIFI_PASS fields
   4. Run code
      
   <blockquote style="background-color: #ffffcc; border-left: 5px solid #ffeb3b; padding: 10px;">
    <strong>Important:</strong> You may run into a sketch is too big error in aruino, to solve this open the Aruino IDE go to tools >> partition scheme and set it to Minimal SPIFFS (1.9MB with OTA/190KB SPIFFS)
</blockquote>

