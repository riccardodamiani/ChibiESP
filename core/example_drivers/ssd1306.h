#ifndef SSD1306_DISPLAY_H
#define SSD1306_DISPLAY_H

#include "core/kernel/driver/display_driver.h"

class Adafruit_SSD1306;

class SSD1306 : public DisplayDriver{
public:
    SSD1306();
    int configure(void* arg) override;
    int init(DisplayDriverInitStruct *chibiESP_Instance) override;
    int deinit(void* arg) override;
    int clearScreen();
    int updateScreen();
    int drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color) override;
    int getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height) override;
private:
    int _screenWidth, _screenHeight;
    Adafruit_SSD1306* _displayObj;

};

#endif  //SSD1306_DISPLAY_H