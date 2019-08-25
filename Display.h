#ifndef Display_h
#define Display_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

class Display {
  public:
    // Constructor(s)
    Display();

    void begin();
    void write(const char* lable, const char* value);
    void clear();
 
  private:
    Adafruit_SSD1306 *display;
};

#endif
