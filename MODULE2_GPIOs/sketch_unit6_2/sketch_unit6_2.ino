#define timeSeconds 10

const int led = 26;
const int motionSensor = 27;

unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

void setup() {
  Serial.begin(115200);

  pinMode(motionSensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
}

void loop() {
  now = millis();
  if (startTimer && (now - lastTrigger > (timeSeconds * 1000))) {
    Serial.println("Motion stopped...");
    digitalWrite(led, LOW);
    startTimer = false;
  }
}

void detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(led, HIGH);
  startTimer = true;
  lastTrigger = millis();
}
