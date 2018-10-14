#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define bleServerName "LED Blink BLE for ESP32"

#define SERVICE_UUID "15f49f4a-37b2-4a07-99f7-7730a9e98096"
#define CHARACTERISTIC_UUID "d391fd60-0169-424a-ae00-9d56f1b46481"

const int ledPin1 = 4;
const int ledPin2 = 16;
const int ledPin3 = 17;
const int ledPin4 = 5;

bool deviceConnected = false;

// prototype
void ledType1(void);
void ledType2(void);
void ledType3(void);
void ledType4(void);
void ledType5(void);

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer *pServer) {
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

      if (rxValue.find("ONE") != -1) {
        Serial.println(" - ONE");
        ledType1();
      } else if (rxValue.find("TWO") != -1) {
        Serial.println(" - TWO");
        ledType2();
      } else if (rxValue.find("THREE") != -1) {
        Serial.println(" - THREE");
        ledType3();
      } else if (rxValue.find("FOUR") != -1) {
        Serial.println(" - FOUR");
        ledType4();
      } else {
        Serial.println(" - FIVE(OTHER)");
        ledType5();
      }
    }
  }
};

void setup() {
  Serial.begin(115200);

  // pin mode setup
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  // ble setup
  BLEDevice::init(bleServerName);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting to connect...");
}

void loop() {

}

void ledType1() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, HIGH);
}

void ledType2() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, HIGH);
  digitalWrite(ledPin4, LOW);
}

void ledType3() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, HIGH);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, HIGH);
}

void ledType4() {
  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, HIGH);
}

void ledType5() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);
}
