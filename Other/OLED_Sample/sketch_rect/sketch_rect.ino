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
  display.drawRect(10, 10, 44, 44, WHITE);
  display.fillRect(74, 10, 44, 44, WHITE);
  display.display();
  delay(1000);

  display.clearDisplay();
  display.fillRect(10, 10, 44, 44, WHITE);
  display.drawRect(74, 10, 44, 44, WHITE);
  display.display();
  delay(1000);
}
