#ifndef Periphirals_H
#define Periphirals_H

#include "DHT.h"
#define DHTPIN 27
#define MQPIN 34
#define LDRPIN 14
#define LDRLEDPIN 5
#define REDLEDPIN 15
#define GREENLEDPIN 22
#define BLUELEDPIN 23
#define BUZZPIN 5
#define RED 1
#define BLUE 2
#define GREEN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHT11);

class Periphirals{
    public:
        Periphirals(){
            pinMode(REDLEDPIN,   OUTPUT);
            pinMode(GREENLEDPIN, OUTPUT);
            pinMode(BLUELEDPIN,  OUTPUT);

            pinMode(LDRLEDPIN,OUTPUT);
        }
        float getTemp(){
        float t = dht.readTemperature();
        if (isnan(t)) {
          Serial.println(F("Failed to read!"));
          return -1.0;
        }
        return t;
        }

        float getHumidity(){
          float h = dht.readHumidity();
          if (isnan(h)) {
            Serial.println(F("Failed to read!"));
            return -1.0;
          }
          return h;
        }

        int getGas(){
          int g = analogRead(MQPIN);
          if (isnan(g)) {
            Serial.println(F("Failed to read!"));
            return -1.0;
          }
          return g;
        }

        int getLight(){
            int l = digitalRead(LDRPIN);
            if (isnan(l)) {
              Serial.println(F("Failed to read!"));
            return -1.0;
          }
          return l;

        }

        void LightLED(bool on){
          if(on){
            digitalWrite(LDRLEDPIN,HIGH);
          }
          else{
            digitalWrite(LDRLEDPIN,LOW);
          }
        }

        void rgbLED(int rgb){
          if(rgb == RED){
            analogWrite(REDLEDPIN,   255);
            analogWrite(BLUELEDPIN,   0);
            analogWrite(GREENLEDPIN,   0);
          }else if(rgb == BLUE){
            analogWrite(REDLEDPIN,   0);
            analogWrite(BLUELEDPIN,   255);
            analogWrite(GREENLEDPIN,   0);
          }else if(rgb == GREEN){
            analogWrite(REDLEDPIN,   0);
            analogWrite(BLUELEDPIN,   0);
            analogWrite(GREENLEDPIN,   255);
          }else{
            analogWrite(REDLEDPIN,   0);
            analogWrite(BLUELEDPIN,   0);
            analogWrite(GREENLEDPIN,   0);
          }
        }
        void buzz(bool on){
          if(on){
            digitalWrite(BUZZPIN,HIGH);
          }
          else{
            digitalWrite(BUZZPIN,LOW);
          }
        }
};

#endif 
