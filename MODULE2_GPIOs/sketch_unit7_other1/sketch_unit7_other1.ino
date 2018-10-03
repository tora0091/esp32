const int buttonPin = 4;
const int ledPin = 16;

int buttonState = LOW;
int ledState = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if (buttonState != LOW) {
    ledState = !ledState;
  }
  digitalWrite(ledPin, ledState);
  delay(500);
}
