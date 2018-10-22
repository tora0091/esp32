#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

Adafruit_SSD1306 display(-1);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
}

void loop() {
  display.clearDisplay();
  display.drawTriangle(32, 25, 57, 50, 7, 50, WHITE);
  display.fillTriangle(96, 25, 121, 50, 71, 50, WHITE);
  display.display();
  delay(1000);
}
