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
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Good");
  display.println("Morning");

  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      display.invertDisplay(true);
    } else {
      display.invertDisplay(false);
    }
    display.display();
    delay(1000);
  }
}
