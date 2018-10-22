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
  display.drawPixel(10, 10, WHITE);
  display.drawPixel(117, 10, WHITE);
  display.drawPixel(117, 53, WHITE);
  display.drawPixel(10, 53, WHITE);
  display.display();
  delay(1000);
}
