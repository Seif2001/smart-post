#include "Periphirals.h"
#include "BLE.h"

  void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11 Test"));
  setupBLE();
  setupPeripherals();
 

}

void loop() {
  delay(500);
  Serial.print("Gas: ");
  Serial.print(getGas());
  int gas = getGas();
  int hum = getHumidity();
  int temp = getTemp();
  notifyHumidity(hum);
  notifyTemp(temp);
  notifyGas(gas);
  disconnect();
  reconnect();

  Serial.print("Light: ");
  Serial.print(getLight());
  rgbLED(GREEN);
  LightLED(0);
  Serial.println();
  delay(1000);
}