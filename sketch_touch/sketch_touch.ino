const int LED_PIN = 14;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (touchRead(T0) < 10) {
    Serial.println("touch!!!!");
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);    
  }
  delay(500);
}
