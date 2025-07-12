// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef SSD1306_DISPLAY_H
#define SSD1306_DISPLAY_H

#include "core/kernel/device/display_device.h"

class Adafruit_SSD1306;

struct SSD1306ConfigStruct {
    int screenWidth;  // Width of the display in pixels
    int screenHeight; // Height of the display in pixels
    uint8_t i2c_bus; // I2C bus number
};

class SSD1306 : public DisplayDevice{
public:
    SSD1306(uint32_t deviceId);
    int configure(SSD1306ConfigStruct config);
    int init() override;
    int deinit(void* arg) override;
    int get_device_info(DisplayDeviceInfo_t &info) override;
    int clearScreen() override;
    int updateScreen() override;
    int drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color) override;
    int getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height) override;
    
private:
    int _screenWidth, _screenHeight;
    uint8_t _i2c_bus; // I2C bus number
    Adafruit_SSD1306* _displayObj;

};

#endif  //SSD1306_DISPLAY_H