const int ledPin = 2;
const int delayTime = 1000;

unsigned long now;
unsigned long lastTime = millis();

int ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
}

void loop() {
  now = millis();
  if ((now - lastTime) > delayTime) {
    lastTime = now;
    ledState = !ledState;
  }
  digitalWrite(ledPin, ledState);
}
