/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "Periphirals.h"
BLEServer* pServer = NULL;
BLECharacteristic* pGasCharacteristic = NULL;
BLECharacteristic* pTempCharacteristic = NULL;
BLECharacteristic* pHumCharacteristic = NULL;
BLECharacteristic* pEMGASCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define GAS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define EMGAS_CHARACTERISTIC_UUID "1e10fc9d-609a-4347-bd5c-4e9f3f9340c6"
#define TEMP_CHARACTERISTIC_UUID "e3a16c97-4ddd-4876-b528-d96534ed15ad"
#define HUM_CHARACTERISTIC_UUID "34aec8a6-b476-4f3d-8b5f-b3f45d54f61a"



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setupBLE(){
  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pGasCharacteristic = pService->createCharacteristic(
                      GAS_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // Create a BLE Descriptor
  pGasCharacteristic->addDescriptor(new BLE2902());

  // Create a BLE Characteristic
  pEMGASCharacteristic = pService->createCharacteristic(
                      EMGAS_CHARACTERISTIC_UUID,

                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE 
                    );

  // Create a BLE Descriptor
  pEMGASCharacteristic->addDescriptor(new BLE2902());

  pTempCharacteristic = pService->createCharacteristic(
                      TEMP_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // Create a BLE Descriptor
  pTempCharacteristic->addDescriptor(new BLE2902());

  pHumCharacteristic = pService->createCharacteristic(
                      HUM_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

  // Create a BLE Descriptor
  pHumCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void notifyAll(float h, float t, int gas){
  // notify changed value
    if (deviceConnected) {
        std::string emVal_str = pEMGASCharacteristic->getValue();
        uint8_t emVal = emVal_str[0];
        Serial.print("EmergencyValue: ");
        Serial.println(emVal);
        lightEmergency(emVal);
        pGasCharacteristic->setValue((uint8_t*)&gas, 4);
        pGasCharacteristic->notify();
        pTempCharacteristic->setValue(t);
        pTempCharacteristic->notify();
        pHumCharacteristic->setValue(h);
        pHumCharacteristic->notify();
        delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

void setup() {
  setupPeripherals();
  Serial.begin(115200);
  setupBLE();
}

void loop() {
    float t = getTemp();
    float h = getHumidity();
    int g = getGas();
    Serial.print("Gas: ");
    Serial.println(g);
    Serial.print("Temp: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    delay(1000);
    notifyAll(h, t, g);
}