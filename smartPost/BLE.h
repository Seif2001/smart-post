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



// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define TEMP_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define HUMIDITY "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define GAS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"



class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class BLE{
    public:
        BLEServer* pServer = NULL;
        BLECharacteristic* pTempCharacteristic = NULL;
        BLECharacteristic* pHumidityCharacteristic = NULL;
        BLECharacteristic* pGasCharacteristic = NULL;
        bool deviceConnected = false;
        bool oldDeviceConnected = false;

        BLE(std::string name){
            // Create the BLE Device
            BLEDevice::init(name);
        }
        void createServer(){
            // Create the BLE Server
            pServer = BLEDevice::createServer();
            pServer->setCallbacks(new MyServerCallbacks());
        }
        void createService(){
            // Create the BLE Service
            BLEService *pService = pServer->createService(SERVICE_UUID);
        }
        void createTempCharacteristic(){
            // Create a BLE Characteristic
            pTempCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
            // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
            // Create a BLE Descriptor
            BLE2902 *pBLE2902;
            pBLE2902 =  = new BLE2902();
            pBLE2902->setNotifications(true);
            BLEDescriptor *pDescr;
            pDescr = new BLEDescriptor((uint16_t) 0x2901);
            pDescr->setValue("Temp")

            pTempCharacteristic->addDescriptor(pBLE2902);
            pTempCharacteristic->addDescriptor(pDescr);

        }

        void createHumidityCharacteristic(){
            // Create a BLE Characteristic
            pHumidityCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
            // Create a BLE Descriptor
            BLE2902 *pBLE2902;
            pBLE2902 =  = new BLE2902();
            pBLE2902->setNotifications(true);
            BLEDescriptor *pDescr;
            pDescr = new BLEDescriptor((uint16_t) 0x2901);
            pDescr->setValue("Humidity")

            pHumidityCharacteristic->addDescriptor(pBLE2902);
            pHumidityCharacteristic->addDescriptor(pDescr);
        }

        void createGasCharacteristic(){
            // Create a BLE Characteristic
            pGasCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
            // Create a BLE Descriptor
            BLE2902 *pBLE2902;
            pBLE2902 =  = new BLE2902();
            pBLE2902->setNotifications(true);
            BLEDescriptor *pDescr;
            pDescr = new BLEDescriptor((uint16_t) 0x2901);
            pDescr->setValue("Gas")

            pGasCharacteristic->addDescriptor(pDescr);
            pGasCharacteristic->addDescriptor(pBLE2902);
        }

        void startAdvertising(){
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

        void notify(int value){
             // notify changed value
            if (deviceConnected) {
                pCharacteristic->setValue(value);
                pCharacteristic->notify();
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
};

