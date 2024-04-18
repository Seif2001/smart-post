#include "Periphirals.h"


Periphirals periph;

void setup() {
  periph = Periphirals();
  Serial.begin(9600);
  Serial.println(F("DHT11 Test"));
  dht.begin();
}

void loop() {
  delay(500);
  Serial.print("Gas: ");
  Serial.print(periph.getGas());
  Serial.print("Light: ");
  Serial.print(periph.getLight());
  periph.rgbLED(GREEN);
  periph.LightLED(0);
  Serial.println();
}