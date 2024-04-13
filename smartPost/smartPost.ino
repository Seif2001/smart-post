#include "DHT.h"

#define DHTPIN 4   
#define DHTTYPE DHT11 
int digitalGas = 25;
int analogGas = 33;
int digitalLDR = 27;
int analogLDR = 14;
int buzzerOut = 21;
int ledOut = 16;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(digitalGas, INPUT);
  pinMode(digitalLDR, INPUT);
  pinMode(buzzerOut, OUTPUT);
  pinMode(ledOut, OUTPUT);


  Serial.begin(9600);
  // Serial.println(F("Monitor is open."));
  //dht.begin();
}


void checkTemp(float temperature, float humidity){
  if(temperature > 32 && humidity > 40){
    digitalWrite(buzzerOut,HIGH);
  }
  else{
    digitalWrite(buzzerOut,LOW);
  }
}

void checkLDR(int LDRAnalog){
  if(LDRAnalog < 0.4*4095){ //dont forget its active low (range is from 0-4095)
    digitalWrite(ledOut,HIGH);
  }
  else{
    digitalWrite(ledOut,LOW);

  }
}

void checkGas(float temperature, float humidity){
  //
}

void loop() {
  delay(2000);
  bool digital_gas_read = digitalRead(digitalGas);
  int analog_gas_read = analogRead(analogGas);
  bool digital_LDR_read = digitalRead(digitalLDR); //active low
  int analog_LDR_read = analogRead(analogLDR);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  // if (isnan(h) || isnan(t) || isnan(f)) {
  //   Serial.println(F("Failed to read from DHT sensor!"));
  //   return;
  // }
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


  Serial.print("Gas Analog Value : ");
  Serial.print(analog_gas_read);
  Serial.print("t");
  Serial.print("Gas Digital value :");
  Serial.println(digital_gas_read);
  Serial.print("LDR Analog Value : ");
  Serial.print(analog_LDR_read);
  Serial.print("LDR Digital value :");
  Serial.println(digital_LDR_read);
  checkTemp(t,h);
  checkLDR(analog_LDR_read);
}