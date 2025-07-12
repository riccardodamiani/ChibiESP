// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DISPLAY_DEVICE_H
#define DISPLAY_DEVICE_H

#include <string>
#include <stdint.h>

class ChibiESP;

enum BW_Color{
    CESP_BLACK = 0,
    CESP_WHITE = 1
};

struct RGB_Color{
    uint8_t r, g, b;
};

enum class DisplayColorType {
    Monochrome,    // 1 bit per pixel
    Grayscale,     // es. 8 bit per pixel
    RGB,           // es. 16 o 24 bit
    Unknown        // fallback
};

struct DisplayDeviceInfo_t{
    uint16_t screenWidth, screenHeight;
    DisplayColorType colorType;
    uint16_t colorDepth; // in bits
    std::string displayModel;
    uint32_t controllerId;
};

class DisplayDevice{
public:
    DisplayDevice(uint32_t deviceId);
    DisplayDevice() = delete;
    virtual int init();
    virtual int deinit(void* arg);
    virtual int updateScreen();
    virtual int clearScreen();

    //functions for monochromatic displays
    virtual int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color);
    virtual int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color);
    virtual int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color);
    virtual int drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color);
    virtual int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color);
    virtual int fillScreen(BW_Color color);

    //functions for colored displays
    virtual int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color);
    virtual int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color);
    virtual int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color);
    virtual int drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color);
    virtual int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color);
    virtual int fillScreen(RGB_Color color);

    //info functions
    virtual int getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height);
    virtual int get_device_info(DisplayDeviceInfo_t &info);
    uint32_t get_device_id() const { return _deviceId; }
private:
    uint32_t _deviceId; // Device ID
};

#endif  //DISPLAY_DEVICE_H