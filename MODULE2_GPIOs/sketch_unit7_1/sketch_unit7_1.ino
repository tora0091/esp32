#include <EEPROM.h>

#define EEPROM_SIZE 1

const int buttonPin = 4;
const int ledPin = 16;

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);

  EEPROM.begin(EEPROM_SIZE);

  ledState = EEPROM.read(0);
  digitalWrite(ledPin, ledState);
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }
  lastButtonState = reading;

  if (digitalRead(ledPin) != ledState) {
    Serial.println("State changed");
    digitalWrite(ledPin, ledState);

    EEPROM.write(0, ledState);
    EEPROM.commit();
    Serial.println("State saved in flash memory");
  }
}
