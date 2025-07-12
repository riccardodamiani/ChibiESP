// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/base_devices/ssd1306.h"
#include "core/logging/logging.h"
#include "core/kernel/device/display_device.h"
#include "chibiESP.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

SSD1306::SSD1306(uint32_t deviceId) :
    DisplayDevice(deviceId),
    _displayObj(nullptr)
{

}

int SSD1306::configure(SSD1306ConfigStruct config){
    _screenWidth = config.screenWidth;
    _screenHeight = config.screenHeight;
    _i2c_bus = config.i2c_bus;
    return 0;
}

int SSD1306::init(){
    //This device needs a i2c interface handled by the kernel
    //NEVER close the TwoWire interface since it's managed by the system
    TwoWire* i2cInterface = chibiESP.getI2cInterface(_i2c_bus);
    if(i2cInterface == nullptr){
        Logger::error("SSD1306 device error: i2c interface is nullptr");
        return -1;
    }

    _displayObj = new Adafruit_SSD1306(_screenWidth, _screenHeight, i2cInterface, -1);
    if (!_displayObj->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Logger::error("SSD1306 device error: could not initialize display");
        return -1;
    }
    return 0;
}

int SSD1306::deinit(void* arg){
    if(_displayObj == nullptr) return 0;
    
    delete _displayObj;
    _displayObj = nullptr;
    return 0;
}

int SSD1306::get_device_info(DisplayDeviceInfo_t &info){
    info.screenWidth = _screenWidth;
    info.screenHeight = _screenHeight;
    info.colorType = DisplayColorType::Monochrome;
    info.colorDepth = 1; // 1 bit per pixel
    info.displayModel = "SSD1306";
    info.controllerId = 0;
    return 0;
}

int SSD1306::updateScreen(){
    _displayObj->display();
    return 0;
}

int SSD1306::clearScreen(){
    _displayObj->clearDisplay();
    return 0;
}

int SSD1306::drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color){
    int16_t x1, y1;
    uint16_t w, h;

    //get text size
    _displayObj->setTextSize(size);
    _displayObj->getTextBounds(text.c_str(), x, y, &x1, &y1, &w, &h);

    //Draw the background color
    _displayObj->fillRect(x1, y1, w, h, bg_color);

    // draw the text
    _displayObj->setTextColor(fg_color);
    _displayObj->setCursor(x, y);
    _displayObj->print(text.c_str());

    return 0;
}

int SSD1306::getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t *width, uint16_t *height){
    _displayObj->setTextSize(size);
    _displayObj->getTextBounds(text.c_str(), x, y, real_x, real_y, width, height);
    return 0;
}
