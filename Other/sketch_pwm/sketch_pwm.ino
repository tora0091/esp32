const int pwmPin = 4;

const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(pwmPin, ledChannel);
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    ledcWrite(ledChannel, i);
    delay(15);
  }
}
