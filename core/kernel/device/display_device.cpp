// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

/**
 * @file bw_display_device.cpp
 * @brief Display device template
 * @details Template for a generic display device. overload this functions to a device for a specific display
 */

#include "core/kernel/device/display_device.h"

DisplayDevice::DisplayDevice(uint32_t deviceId) : 
    _deviceId(deviceId)
{

}

/**
 * @brief TODO: Function to get device information
 */
int DisplayDevice::get_device_info(DisplayDeviceInfo_t &info){
    return 0;
}

/**
 * @brief TODO: Function to get the size in pixels the text will have on screen
 */
int DisplayDevice::getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height){
    return 0;
}


/**
 * @brief device initialization function
 * @details This is called by the kernel when the device is loaded (and starts running). This should contain hardware inizialization stuff
 */
int DisplayDevice::init(){
    return 0;
}

/**
 * @brief device de-initialization function
 * @details This is called by the kernel when the device is unloaded (stops running). This should contain hardware de-initialization stuff, memory cleaning, ecc..
 */
int DisplayDevice::deinit(void* arg){
    return 0;
}

/**
 * @brief device update screen function function
 * @details Draws the internal screen buffer on screen
 */
int DisplayDevice::updateScreen(){
    return 0;
}

/**
 * @brief Clear the internal display buffer
 */
int DisplayDevice::clearScreen(){
    return 0;
}

/*********************************
* Monochromatic display functions
/************************************/

/**
 * @brief Function to draw a line on screen
 */
int DisplayDevice::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a rectangle on screen
 */
int DisplayDevice::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a circle on screen
 */
int DisplayDevice::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a text on screen
 */
int DisplayDevice::drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color){
    return 0;
}

/**
 * @brief Function to draw a bitmap (1 bit per color) on screen
 */
int DisplayDevice::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color){
    return 0;
}

/**
 * @brief Function to fill the screen with one color
 */
int DisplayDevice::fillScreen(BW_Color color){
    return 0;
}


/*********************************
* RGB display functions
/************************************/

/**
 * @brief Function to draw a line on screen
 */
int DisplayDevice::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a rectangle on screen
 */
int DisplayDevice::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a circle on screen
 */
int DisplayDevice::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a text on screen
 */
int DisplayDevice::drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color){
    return 0;
}
/**
 * @brief Function to draw a bitmap (1 bit per color) on screen
 */
int DisplayDevice::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color){
    return 0;
}
/**
 * @brief Function to fill the screen with one color
 */
int DisplayDevice::fillScreen(RGB_Color color){
    return 0;
}
