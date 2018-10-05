TaskHandle_t Task1;
TaskHandle_t Task2;

const int led1 = 2;
const int led2 = 4;

void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  xTaskCreatePinnedToCore(
    Task1code,  // task function
    "Task1",    // name of task
    10000,      // stack size of task
    NULL,       // parameter of task
    1,          // priority of task
    &Task1,     // task handle
    0           // cpu core no
  );
  delay(500);

  xTaskCreatePinnedToCore(
    Task2code,  // task function
    "Task2",    // name of task
    10000,      // stack size of task
    NULL,       // parameter of task
    1,          // priority of task
    &Task2,     // task handle
    1           // cpu core no
  );
  delay(500);
}

void Task1code(void* pvParameters) {
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(1000);
  }
}

void Task2code(void* pvParameters) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for (;;) {
    digitalWrite(led2, HIGH);
    delay(700);
    digitalWrite(led2, LOW);
    delay(700);
  }
}

void loop() {

}
