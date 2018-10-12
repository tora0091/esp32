#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <base64.h>

// wifi
const char* ssid = "your network ssid";
const char* password = "your network password";

// mail
const char* smtpUsername = "smtp user name";
const char* smtpPassword = "smtp user password";
const char* smtpFromAddress = "from e-mail address";
const int smtpPort = 465;
const char* smtpHostname = "smtp hostname";

const char* smtpToAddress = "to e-mail address";
char* subject = "mail subject";
char* content = "mail content";

const uint32_t timeout = 3000;

void setup() {
  Serial.begin(115200);

  // wifi setting 
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

  if (!sendmail()) {
    Serial.println("Fail to send mail.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

bool sendmail() {
  WiFiClientSecure client;

  Serial.printf("Connect to %s\n", smtpHostname);
  if (!client.connect(smtpHostname, smtpPort)) {
    Serial.println("Could not connect to mail server");
    return false;
  }
  if (!readResponse(client, "220")) {
    Serial.println("Connection Error");
    return false;
  }

  client.println("HELO fiend");
  if (!readResponse(client, "250")) {
    Serial.println("Identification Error");
    return false;
  }

  client.println("AUTH LOGIN");
  if (!readResponse(client, "334")) {
    Serial.println("AUTH LOGIN Failed");
    return false;
  }
  client.println(base64::encode(smtpUsername));
  if (!readResponse(client, "334")) {
    Serial.println("AUTH LOGIN Failed");
    return false;
  }
  client.println(base64::encode(smtpPassword));
  if (!readResponse(client, "235")) {
    Serial.println("SMTP AUTH Error");
    return false;
  }

  client.println("MAIL FROM: <" + String(smtpFromAddress) + ">");
  if (!readResponse(client, "250")) {
    Serial.println("MAIL FROM Failed");
    return false;
  }

  client.println("RCPT TO: <" + String(smtpToAddress) + ">");
  if (!readResponse(client, "250")) {
    Serial.println("RCPT TO Failed");
    return false;
  }
  
  client.println("DATA");
  if (!readResponse(client, "354")) {
    Serial.println("SMTP DATA Error");
    return false;  
  }

  client.println("From: <" + String(smtpFromAddress) + ">");
  delay(100);
  client.println("To: <" + String(smtpToAddress) + ">");
  delay(100);
  client.println("Subject: " + String(subject));
  delay(100);
  client.println("Mime-Version: 1.0");
  delay(100);
  client.println("Content-Type: text/html");
  delay(100);
  client.println();
  delay(100);
  client.println(content);
  delay(100);
  client.println(".");
  if (!readResponse(client, "250")) {
    Serial.println("Sending Message Error");
    return false;  
  }

  client.println("QUIT");
  if (!readResponse(client, "221")) {
    Serial.println("QUIT Failed");
    return false;  
  }

  Serial.println("Sending E-Mail Success");
  return true;
}

bool readResponse(WiFiClientSecure &client, const String &target) {
  uint32_t timeStamp = millis();

  // clientが有効になるまで待機、timeoutの時間だけ待つ
  while (1) {
    if (client.available()) {
      break;
    }
    if (millis() - timeStamp > timeout) {
      Serial.println("SMTP Response TIMEOUT!");
      return false;
    }
    delay(1);
  }

  String res = client.readStringUntil('\n');
  res.trim();
  Serial.println(res.c_str());
  if (target != "" && res.indexOf(target) == -1) {
    return false;
  }
  return true;
}
