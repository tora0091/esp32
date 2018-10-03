const int motionPin = 16;

void setup() {
  Serial.begin(115200);

  pinMode(motionPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionPin), helloInterrupt, RISING);
}

void loop() {

}

void helloInterrupt() {
  Serial.println("Hello Interrupt function!!");
}
