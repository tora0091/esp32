#include "BLEDevice.h"

static BLEUUID serviceUUID("C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01");
static BLEUUID    charUUID("B88E098B-E464-4B54-B827-79EB2B150A9F");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

// motor control pin
const int goPin = 4;
const int backPin = 16;
const int leftTurnPin = 17;
const int rightTurnPin = 5;
const int brakePin = 18;

String controlMode = "BR";

bool connectToServer(BLEAddress pAddress) {
    Serial.print("Forming a connection to ");
    Serial.println(pAddress.toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->connect(pAddress);
    Serial.println(" - Connected to server");

    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      return false;
    }
    Serial.println(" - Found our service");

    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      return false;
    }
    Serial.println(" - Found our characteristic");
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      Serial.print("Found our device!  address: "); 
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }
  }
};

void setup() {
  Serial.begin(115200);

  // BLE setup
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  // pin mode setup
  pinMode(goPin, INPUT);
  pinMode(backPin, INPUT);
  pinMode(leftTurnPin, INPUT);
  pinMode(rightTurnPin, INPUT);
  pinMode(brakePin, INPUT);
}

void loop() {
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  if (connected) {
    if (goPin == HIGH) {
      controlMode = "GO";
    } else if (backPin == HIGH) {
      controlMode = "BK";
    } else if (leftTurnPin == HIGH) {
      controlMode = "LT";
    } else if (rightTurnPin == HIGH) {
      controlMode = "RT";
    } else {
      // break or nothing
      controlMode = "BR";
    }
    pRemoteCharacteristic->writeValue(controlMode.c_str(), controlMode.length());
  }
  delay(500);
}
