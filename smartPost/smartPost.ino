#include "Periphirals.h"
#include "BLE.h"

Periphirals periph;
  void setup() {
  periph = Periphirals();
  Serial.begin(115200);
  Serial.println(F("DHT11 Test"));
  setupBLE();
  dht.begin();
 

}

void loop() {
  delay(500);
  Serial.print("Gas: ");
  Serial.print(periph.getGas());
  int gas = periph.getGas();
  int hum = periph.getHumidity();
  int temp = periph.getTemp();
  notifyHumidity(hum);
  notifyTemp(temp);
  notifyGas(gas);
  disconnect();
  reconnect();

  Serial.print("Light: ");
  Serial.print(periph.getLight());
  periph.rgbLED(GREEN);
  periph.LightLED(0);
  Serial.println();
  delay(1000);
}