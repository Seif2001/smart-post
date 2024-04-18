#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define TEMP_CHARACTERISTIC_UUID "69f6ef8f-2da1-4c20-911d-f2f8077ab9d2"
#define HUMIDITY_CHARACTERISTIC_UUID "0bbce066-e753-456c-b2da-27107f628b6f"
#define GAS_CHARACTERISTIC_UUID "53bbbd34-5400-425a-8841-a555d99b4898"

bool deviceConnected = false;
bool oldDeviceConnected = false;
BLEServer* pServer = NULL;
BLECharacteristic* pTempCharacteristic = NULL;
BLECharacteristic* pHumidityCharacteristic = NULL;
BLECharacteristic* pGasCharacteristic = NULL;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void notifyTemp(int value) {
    if (deviceConnected) {
        pTempCharacteristic->setValue(value);
        pTempCharacteristic->notify();
        delay(1000); 
    }
}

void notifyHumidity(int value) {
    if (deviceConnected) {
        pHumidityCharacteristic->setValue(value);
        pHumidityCharacteristic->notify();
        delay(1000); 
    }
}

void notifyGas(int value) {
    if (deviceConnected) {
        pGasCharacteristic->setValue(value);
        pGasCharacteristic->notify();
        delay(1000); 
    }
}

void disconnect() {
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
}

void reconnect() {
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
}

void setupBLE() {
    BLEDevice::init("ESP32_BLE_Server");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

    pTempCharacteristic = pService->createCharacteristic(
        TEMP_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );

    BLEDescriptor *pTempDescriptor = new BLEDescriptor((uint16_t)0x2901);
    pTempDescriptor->setValue("Temp");
    pTempCharacteristic->addDescriptor(pTempDescriptor);

    BLE2902 *pTemp2902 = new BLE2902();
    pTemp2902->setNotifications(true);
    pTempCharacteristic->addDescriptor(pTemp2902);

    pHumidityCharacteristic = pService->createCharacteristic(
        HUMIDITY_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );

    BLEDescriptor *pHumidityDescriptor = new BLEDescriptor((uint16_t)0x2901);
    pHumidityDescriptor->setValue("Humidity");
    pHumidityCharacteristic->addDescriptor(pHumidityDescriptor);

    BLE2902 *pHumidity2902 = new BLE2902();
    pHumidity2902->setNotifications(true);
    pHumidityCharacteristic->addDescriptor(pHumidity2902);

    pGasCharacteristic = pService->createCharacteristic(
        GAS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );

    BLEDescriptor *pGasDescriptor = new BLEDescriptor((uint16_t)0x2901);
    pGasDescriptor->setValue("Gas");
    pGasCharacteristic->addDescriptor(pGasDescriptor);

    BLE2902 *pGas2902 = new BLE2902();
    pGas2902->setNotifications(true);
    pGasCharacteristic->addDescriptor(pGas2902);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}
