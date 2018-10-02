void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 Touch Test!!");
}

void loop() {
  Serial.println(touchRead(4));
  delay(1000);
}
