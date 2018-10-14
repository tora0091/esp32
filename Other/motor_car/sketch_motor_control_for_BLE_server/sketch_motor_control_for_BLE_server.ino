#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID "C6FBDD3C-7123-4C9E-86AB-005F1A7EDA01"
#define CHARACTERISTIC_UUID_RX "B88E098B-E464-4B54-B827-79EB2B150A9F"

// Motor Driver 1
const int md1PwmPin = 4;
const int md1In1Pin = 16;
const int md1In2Pin = 17;

// Motor Driver 2
const int md2PwmPin = 12;
const int md2In1Pin = 14;
const int md2In2Pin = 27;

bool deviceConnected = false;

// prototype
void go();
void back();
void leftTurn();
void rightTurn();
void brake();

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  };
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();
    if (rxValue.length() > 0) {
      Serial.print("Received value: ");
      for (int i = 0; i < rxValue.length(); i++) {
        Serial.print(rxValue[i]);
      }

      if (rxValue.find("GO") != -1) {
        Serial.println(" - GO");
        go();
      }
      else if (rxValue.find("BK") != -1) {
        Serial.println(" - BK");
        back();
      }
      else if (rxValue.find("LT") != -1) {
        Serial.println(" - LT");
        leftTurn();
      }
      else if (rxValue.find("RT") != -1) {
        Serial.println(" - RT");
        rightTurn();
      }
      else if (rxValue.find("BR") != -1) {
        Serial.println(" - BR");
        brake();
      }
    }
  }
};

void setup() {
  Serial.begin(115200);

  // Motor driver 1 setting
  pinMode(md1PwmPin, OUTPUT);
  pinMode(md1In1Pin, OUTPUT);
  pinMode(md1In2Pin, OUTPUT);

  // Motor driver 2 setting
  pinMode(md2PwmPin, OUTPUT);
  pinMode(md2In1Pin, OUTPUT);
  pinMode(md2In2Pin, OUTPUT);
  
  digitalWrite(md1PwmPin, HIGH);
  digitalWrite(md2PwmPin, HIGH);

  // BLE setting
  BLEDevice::init("ESP32_Board");

  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic* pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID_RX,
                                        BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting to connect...");
}

void loop() {

}

void go() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
}

void back() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
}

void rightTurn() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
}

void leftTurn() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
}

void brake() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, HIGH);
}
