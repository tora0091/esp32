#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// BME280 wire setting
const int SDA_BME = 5;
const int SCL_BME = 4;

// OLED wire setting
const int SDA_OLED = 21;
const int SCL_OLED = 22;

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h")
#endif

Adafruit_BME280 bme;
Adafruit_SSD1306 display(-1);

void setup() {
  Serial.begin(115200);

  // BME280 wire
  Wire1.begin(SDA_BME, SCL_BME);

  // OLED wire
  Wire.begin(SDA_OLED, SCL_OLED);

  // BME280 start
  bool status;
  status = bme.begin(0x76, &Wire1);
  delay(150);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  // OLED start
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
}

void loop() {
  Serial.print("Temp: ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  // print OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Temp: ");
  display.print(bme.readTemperature());
  display.println(" *C");
  display.display();

  delay(3000);
}
