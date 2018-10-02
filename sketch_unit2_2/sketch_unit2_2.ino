const int touchPin = 4;
const int ledPin = 16;

int touchState = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  touchState = touchRead(touchPin);
  if (touchState < 20) {
    Serial.println("touch!!");
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
