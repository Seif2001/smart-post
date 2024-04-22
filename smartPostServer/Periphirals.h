#ifndef Periphirals_H
#define Periphirals_H

#include "DHT.h"
#define DHTPIN 25
#define MQPIN 34
#define LDRPIN 14
#define LDRLEDPIN 32
#define REDLEDPIN 15
#define GREENLEDPIN 22
#define BLUELEDPIN 23
#define BUZZPIN 18
#define RED 1
#define BLUE 2
#define GREEN 3
#define EMPIN 26
#define DHTTYPE DHT11





void setupPeripherals(){
    pinMode(LDRLEDPIN,OUTPUT);
  pinMode(REDLEDPIN,   OUTPUT);
  pinMode(GREENLEDPIN, OUTPUT);
  pinMode(BLUELEDPIN,  OUTPUT);
  pinMode(BUZZPIN,OUTPUT);
  pinMode(EMPIN,OUTPUT);
}
void lightEmergency(bool on){
  if(on){
    digitalWrite(EMPIN,HIGH);
  }
  else{
    digitalWrite(EMPIN,LOW);
  }
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

void lightLed(bool on){
  if(on){
    digitalWrite(LDRLEDPIN,HIGH);
  }
  else{
    digitalWrite(LDRLEDPIN,LOW);
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

void checkTemp(float temperature, float humidity){
  if(temperature > 32 && humidity > 40){
    buzz(true);
  }
  else{
    buzz(false);

  }
}

void checkLDR(int LDRAnalog){
  if(LDRAnalog == 0){ //dont forget its active low (range is from 0-4095)
    Serial.println("Room is Light");
    lightLed(false);
  }
  else{
  Serial.println("Room is Dark");
    lightLed(true);

  }
}

void rgbLED(int rgb){
  if(rgb == RED){
    digitalWrite(REDLEDPIN, HIGH);
    digitalWrite(BLUELEDPIN, LOW);
    digitalWrite(GREENLEDPIN, LOW);


  }else if(rgb == BLUE){
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, HIGH);
    digitalWrite(GREENLEDPIN, LOW);
  }else if(rgb == GREEN){
    digitalWrite(REDLEDPIN, LOW);
    digitalWrite(BLUELEDPIN, LOW);
    digitalWrite(GREENLEDPIN, HIGH);
  }
}
void checkGas(int gas){
  if(gas<=400){
    rgbLED(GREEN);
  }else if(gas>400 && gas<1000){
    rgbLED(BLUE);
  }
  else{
    rgbLED(RED);
  }
}


#endif 
