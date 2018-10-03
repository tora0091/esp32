#define BUTTON_PIN_BITMASK 0x200000000

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case 1 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case 2 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case 3 : Serial.println("Wakeup caused by timer"); break;
    case 4 : Serial.println("Wakeup caused by touch"); break;
    case 5 : Serial.println("Wakeup caused by ULP program"); break;
    default: Serial.println("Wakeup was not caused by deep sleep"); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1);

  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop() {

}
