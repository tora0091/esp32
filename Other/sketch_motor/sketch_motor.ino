/**
 * Test Motor
 */

// Motor Driver 1
const int md1PwmPin = 4;
const int md1In1Pin = 16;
const int md1In2Pin = 17;

// Motor Driver 2
const int md2PwmPin = 12;
const int md2In1Pin = 14;
const int md2In2Pin = 27;

void setup() {
  Serial.begin(115200);

  // Motor driver 1 setting
  pinMode(md1PwmPin, OUTPUT);
  pinMode(md1In1Pin, OUTPUT);
  pinMode(md1In2Pin, OUTPUT);

  // Motor driver 2 setting
  pinMode(md2PwmPin, OUTPUT);
  pinMode(md2In1Pin, OUTPUT);
  pinMode(md2In2Pin, OUTPUT);
  
  digitalWrite(md1PwmPin, HIGH);
  digitalWrite(md2PwmPin, HIGH);
}

void loop() {
  go();
  delay(5000);
  brake();
  delay(5000);

  back();
  delay(5000);
  brake();
  delay(5000);

  rightTurn();
  delay(5000);
  brake();
  delay(5000);

  leftTurn();
  delay(5000);
  brake();
  delay(5000);
}

void go() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
  delay(500);
}

void back() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
  delay(500);  
}

void rightTurn() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
  delay(500);
}

void leftTurn() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
  delay(500);
}

void brake() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, HIGH);
  delay(500);
}
