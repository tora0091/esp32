#include <BLEDevice.h>

static BLEUUID serviceUUID("15f49f4a-37b2-4a07-99f7-7730a9e98096");
static BLEUUID    charUUID("d391fd60-0169-424a-ae00-9d56f1b46481");

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

const int ctlPin1 = 4;
const int ctlPin2 = 16;
const int ctlPin3 = 17;
const int ctlPin4 = 5;

int ctlPin1State = LOW;
int ctlPin2State = LOW;
int ctlPin3State = LOW;
int ctlPin4State = LOW;

String controlMode = "ONE";

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

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);

  // pin mode setup
  pinMode(ctlPin1, INPUT);
  pinMode(ctlPin2, INPUT);
  pinMode(ctlPin3, INPUT);
  pinMode(ctlPin4, INPUT);
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

    ctlPin1State = digitalRead(ctlPin1);
    ctlPin2State = digitalRead(ctlPin2);
    ctlPin3State = digitalRead(ctlPin3);
    ctlPin4State = digitalRead(ctlPin4);
    
    if (ctlPin1State == HIGH) {
      controlMode = "ONE";
    } else if (ctlPin2State == HIGH) {
      controlMode = "TWO";
    } else if (ctlPin3State == HIGH) {
      controlMode = "THREE";
    } else if (ctlPin4State == HIGH) {
      controlMode = "FOUR";
    } else {
      controlMode = "FIVE";
    }
    Serial.println(controlMode);
    pRemoteCharacteristic->writeValue(controlMode.c_str(), controlMode.length());
  }
  delay(500);
}
