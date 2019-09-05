#include "Display.h"

Display::Display() {
    this->display = new Adafruit_SSD1306(OLED_RESET);
}

void Display::begin() {
    this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->clear();

    this->display->setTextSize(1);
    this->display->setTextColor(WHITE);
}

void Display::write(const char* lable, const char* value) {
    this->clear();
    this->display->setCursor(0,0);
    this->display->println(lable);
    this->display->println(value);
    this->display->display();
}

void Display::clear() {
    this->display->clearDisplay();
    this->display->display();
}
