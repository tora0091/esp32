#include <EEPROM.h>

#define EEPROM_SIZE 1

const int buttonPin = 4;
const int ledPin = 16;

int buttonState = LOW;
int ledState = LOW;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  /* esp32 memory read: led state read */
  ledState = EEPROM.read(0);
  digitalWrite(ledPin, ledState);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState != LOW) {
    ledState = !ledState;

    /* esp32 memory write: led state write */
    EEPROM.write(0, ledState);
    EEPROM.commit();
  }
  digitalWrite(ledPin, ledState);
  delay(500);
}
