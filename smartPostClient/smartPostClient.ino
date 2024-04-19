 /**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
//#include "BLEScan.h"
#include "Periphirals.h"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID    gasCharUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
static BLEUUID    tempCharUUID("e3a16c97-4ddd-4876-b528-d96534ed15ad");
static BLEUUID    humCharUUID("34aec8a6-b476-4f3d-8b5f-b3f45d54f61a");
static BLEUUID    EMGASCharUUID("1e10fc9d-609a-4347-bd5c-4e9f3f9340c6");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLEAdvertisedDevice* myDevice;

BLERemoteCharacteristic *pEMGASCharacterisitic;
uint32_t gas_ble = 0;
static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  if(pBLERemoteCharacteristic->getUUID().toString() == gasCharUUID.toString()){
    gas_ble = pData[0];
    for(int i =1; i<length; i++){
      gas_ble = gas_ble | pData[i] << i*8;
    }
    Serial.print("Remote gas: ");
    Serial.println(gas_ble);
  }

  if(pBLERemoteCharacteristic->getUUID().toString() == tempCharUUID.toString()){
    float tamp_ble = 0.0;
    memcpy(&tamp_ble, pData, sizeof(float));
    Serial.print("Remote temp: ");
    Serial.println(tamp_ble);
  }
  if(pBLERemoteCharacteristic->getUUID().toString() == humCharUUID.toString()){
    float hum_ble = 0.0;
    memcpy(&hum_ble, pData, sizeof(float));
    Serial.print("Remote Humidity: ");
    Serial.println(hum_ble);
  }


}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectChar(BLERemoteService *pRemoteService, BLEUUID charUUID){
// Obtain a reference to the characteristic in the service of the remote BLE server.
  BLERemoteCharacteristic* pRemoteCharacteristic;

  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(gasCharUUID.toString().c_str());
    return false;
  }

  if(pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);
  return true;
}

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    

    connected = true;
    if(connectChar(pRemoteService, gasCharUUID) == false){
      connected = false;
    }
    else if (connectChar(pRemoteService, tempCharUUID) == false){
      connected = false;
    }
    else if(connectChar(pRemoteService, humCharUUID) == false){
      connected = false;
    }
    if(!connected){
      pClient->disconnect();
      Serial.println("At least one char is disconnected");
      return false;
    }
    pEMGASCharacterisitic = pRemoteService->getCharacteristic(EMGASCharUUID);
    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
      Serial.println("OK");

    } // Found our server
      Serial.println("Not OK");

  } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
} // End of setup.


// This is the Arduino main loop function.
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    // String newValue = "Time since boot: " + String(millis()/1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    
    // // Set the characteristic's value to be the array of bytes that is actually a string.
    int avgGas = (gas_ble + getGas())/2;
    Serial.println(gas_ble);
    Serial.println(avgGas);

    if(avgGas>1000){
      Serial.println("HI");
      pEMGASCharacterisitic->writeValue(1);

    }else{
      pEMGASCharacterisitic->writeValue(0);
    }

  }else if(doScan){
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  Serial.print("Our gas: ");
  Serial.println(getGas());
  delay(1000); // Delay a second between loops.
} // End of loop
