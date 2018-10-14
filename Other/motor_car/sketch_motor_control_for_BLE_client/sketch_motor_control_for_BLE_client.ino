#include "BLEDevice.h"

static BLEUUID serviceUUID("c6fbdd3c-7123-4c9e-86ab-005f1a7eda01");
static BLEUUID    charUUID("b88e098b-e464-4b54-b827-79eb2b150a9f");

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

int goPinState = LOW;
int backPinState = LOW;
int leftTurnPinState = LOW;
int rightTurnPinState = LOW;

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

//    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {
      Serial.print("Found our device!  address: "); 
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
//    }
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
    Serial.print("Control Mode : ");

    goPinState = digitalRead(goPin);
    backPinState = digitalRead(backPin);
    leftTurnPinState = digitalRead(leftTurnPin);
    rightTurnPinState = digitalRead(rightTurnPin);
    
    if (goPinState == HIGH) {
      Serial.println("GO");
      controlMode = "GO";
    } else if (backPinState == HIGH) {
      Serial.println("BK");
      controlMode = "BK";
    } else if (leftTurnPinState == HIGH) {
      Serial.println("LT");
      controlMode = "LT";
    } else if (rightTurnPinState == HIGH) {
      Serial.println("RT");
      controlMode = "RT";
    } else {
      // break or nothing
      Serial.println("BR");
      controlMode = "BR";
    }
    Serial.println(controlMode);
    pRemoteCharacteristic->writeValue(controlMode.c_str(), controlMode.length());
  }
  delay(500);
}
