 /**
 * A BLE client example that is rich in capabilities.
 * There is a lot new capabilities implemented.
 * author unknown
 * updated by chegewara
 */

#include "BLEDevice.h"
#include "Periphirals.h"
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define DHTTYPE DHT11
#define DHTPIN 4
#define WIFI_SSID "Tellmywifiloveher"
#define WIFI_PASS "lion2004"
#define API_KEY "AIzaSyDoinkaVM6q3GkhKC9HAvUmEgJLOZAm7vw"
#define DB_URL "https://smart-post-9409b-default-rtdb.asia-southeast1.firebasedatabase.app/"


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;


float t, h, tamp_ble, hum_ble;
int gas_ble;

DHT dht(DHTPIN, DHT11);



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

static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) {
  if(pBLERemoteCharacteristic->getUUID().toString() == gasCharUUID.toString()){
    gas_ble = pData[0];
    for(int i =1; i<length; i++){
      gas_ble = gas_ble | pData[i] << i*8;
    }
    Serial.print(F("Remote gas: "));
    Serial.println(gas_ble);
  }

  if(pBLERemoteCharacteristic->getUUID().toString() == tempCharUUID.toString()){
    tamp_ble = 0.0;
    memcpy(&tamp_ble, pData, sizeof(float));
    Serial.print(F("Remote temp: "));
    Serial.println(tamp_ble);
  }
  if(pBLERemoteCharacteristic->getUUID().toString() == humCharUUID.toString()){
    hum_ble = 0.0;
    memcpy(&hum_ble, pData, sizeof(float));
    Serial.print(F("Remote Humidity: "));
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
    Serial.print(F("Forming a connection to "));
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(F(" - Connected to server"));
    pClient->setMTU(517); //set client to request maximum MTU from server (default is 23 otherwise)
  
    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(F(" - Found our service"));
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
    Serial.print(F("BLE Advertised Device found: "));
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
  setupPeripherals();

  dht.begin();
  Serial.begin(9600);
  Serial.println(F("Starting Arduino BLE Client application..."));
  BLEDevice::init("");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print(F("Connecting"));

  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  config.database_url = DB_URL;

  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Signing in OK");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
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
      Serial.println(F("We are now connected to the BLE Server."));

    } else {
      Serial.println(F("We have failed to connect to the server; there is nothin more we will do."));
    }
    doConnect = false;
  }
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  int avgGas = 0;
  if (connected) {
    // String newValue = "Time since boot: " + String(millis()/1000);
    // Serial.println("Setting new characteristic value to \"" + newValue + "\"");
    // // Set the characteristic's value to be the array of bytes that is actually a string.
    avgGas = (gas_ble + getGas())/2;
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
  t = dht.readTemperature();
    //float t = 5.0;
    float h = dht.readHumidity();
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
  }
  int l = getLight();
  int g = getGas();
  Serial.print(F("Our gas: "));
  Serial.println(g);
  Serial.print(F("Our Temp: "));
  Serial.println(t);
  Serial.print(F("Our Humidity: "));
  Serial.println(h);
  Serial.print(F("Our LDR: "));
  Serial.println(l);
  checkGas(g);
  checkEm(avgGas);
  checkTemp(t,h);
  checkLDR(l);
  delay(2000); // Delay a second between loops.
  if(Firebase.ready()&&signupOK){
    sendDataPrevMillis = millis();
    // --------------------------
    if(Firebase.RTDB.setFloat(&fbdo, "client/temp", t)){
      Serial.print("Sent client temp in " );
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(t);

    }
    if(Firebase.RTDB.setFloat(&fbdo, "client/hum", h)){
      Serial.print("Sent client humidity in ");
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(h);
    }
    if(Firebase.RTDB.setInt(&fbdo, "client/gas", g)){
      Serial.print("Sent client gas in ");
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(g);
    }

    if(Firebase.RTDB.setFloat(&fbdo, "server/temp", tamp_ble)){
      Serial.print("Sent client temp in " );
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(tamp_ble);

    }
    if(Firebase.RTDB.setFloat(&fbdo, "server/hum", hum_ble)){
      Serial.print("Sent client humidity in ");
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(hum_ble);
    }
    if(Firebase.RTDB.setInt(&fbdo, "server/gas", gas_ble)){
      Serial.print("Sent client gas in ");
      Serial.print(fbdo.dataPath());
      Serial.print(" ");
      Serial.println(gas_ble);
    }

  }
} // End of loop




