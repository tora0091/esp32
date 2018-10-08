/**
 * Motor Control
 */
#include <WiFi.h>

const char* ssid = "Game58503";
const char* password = "8468573751106";

WiFiServer server(80);
String header;

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

  // Setting Web Server
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\"content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display:inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border:none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin:2px; cursor: pointer;}");
            client.println(".button2 {background-color:#555555;}</style></head>");
            client.println("<body><h1>ESP32 Motor Control for Web Server</h1>");
            client.println("<p><a href=\"/go\"><button class=\"button\">Go</button></a></p>");
            client.println("<p><a href=\"/back\"><button class=\"button\">Back</button></a></p>");
            client.println("<p><a href=\"/rightTurn\"><button class=\"button\">Right Turn</button></a></p>");
            client.println("<p><a href=\"/leftTurn\"><button class=\"button\">Left Turn</button></a></p>");
            client.println("<p><a href=\"/brake\"><button class=\"button\">Brake</button></a></p>");
            client.println("</body></html>");
            client.println();

            if (header.indexOf("GET /go") >= 0) {
              Serial.println("push button: go");
              go();
            } else if (header.indexOf("GET /back") >= 0) {
              Serial.println("push button: back");
              back();
            } else if (header.indexOf("GET /rightTurn") >= 0) {
              Serial.println("push button: right turn");
              rightTurn();
            } else if (header.indexOf("GET /leftTurn") >= 0) {
              Serial.println("push button: left turn");
              leftTurn();
            } else {
              Serial.println("push button: brake");
              brake();
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void go() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
}

void back() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
}

void rightTurn() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, LOW);
  digitalWrite(md2In1Pin, LOW);
  digitalWrite(md2In2Pin, HIGH);
}

void leftTurn() {
  digitalWrite(md1In1Pin, LOW);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, LOW);
}

void brake() {
  digitalWrite(md1In1Pin, HIGH);
  digitalWrite(md1In2Pin, HIGH);
  digitalWrite(md2In1Pin, HIGH);
  digitalWrite(md2In2Pin, HIGH);
}
