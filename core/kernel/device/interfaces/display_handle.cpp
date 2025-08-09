// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/device/interfaces/display_handle.h"
#include "chibiEsp.h"

DisplayHandle::DisplayHandle(){
    _device = nullptr; // Initialize device pointer to null
    _handle = nullptr; // Initialize handle to null
    // No need to create a handle here, it will be created when requested
}

DisplayHandle::DisplayHandle(DisplayDevice *device, std::unique_ptr<Handle> handle) {
    _device = device;
    _handle = std::move(handle);
}

DisplayHandle::~DisplayHandle(){
    close();
}

/**
 * @brief Closes the display handle.
 * @details Releases the display device. After calling this function the handle cannot be used anymore.
 */
void DisplayHandle::close(){
    if (!_device || !_handle) {
        return; // Nothing to close
    }
    chibiESP.freeDisplayHandle(_device->getDeviceId(), _handle->getId()); // Free the display handle
    _handle.reset(); // Reset the handle to release resources
    _device = nullptr; // Clear the device pointer
}

bool DisplayHandle::getAccessState() const {
    return _device && _handle && _handle->getState() == HandleState::Active; // Check if handle is valid and active
}

/**
 * @brief Updates the screen of the display device.
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::updateScreen(){
    if(!getAccessState()) return -1;
    return _device->updateScreen(); // Call the updateScreen method on the device
}

/**
 * @brief Clears the screen of the display device.
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::clearScreen(){
    if(!getAccessState()) return -1;
    return _device->clearScreen(); // Call the clearScreen method on the device
}


//functions for monochromatic displays
/**
 * @brief Draws a line on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color){
    if(!getAccessState()) return -1;
    return _device->drawLine(x1, y1, x2, y2, color);
}

/**
 * @brief Draws a rectangle on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color){
    if(!getAccessState()) return -1;
    return _device->drawRect(x, y, width, height, fill, color);
}

/**
 * @brief Draws a circle on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color){
    if(!getAccessState()) return -1;
    return _device->drawCircle(x, y, radius, fill, color);
}

/**
 * @brief Draws text on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color){
    if(!getAccessState()) return -1;
    return _device->drawText(text, x, y, size, bg_color, fg_color);
}

/**
 * @brief Draws a bitmap on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color){
    if(!getAccessState()) return -1;
    return _device->drawBitmap(x, y, bitmap, width, height, bg_color, fg_color);
}

/**
 * @brief Fills the screen with a monochromatic color
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::fillScreen(BW_Color color){
    if(!getAccessState()) return -1;
    return _device->fillScreen(color);
}


//functions for colored displays
/**
 * @brief Draws a line on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color){
    if(!getAccessState()) return -1;
    return _device->drawLine(x1, y1, x2, y2, color);
}

/**
 * @brief Draws a rectangle on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color){
    if(!getAccessState()) return -1;
    return _device->drawRect(x, y, width, height, fill, color);
}

/**
 * @brief Draws a circle on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color){
    if(!getAccessState()) return -1;
    return _device->drawCircle(x, y, radius, fill, color);
}

/**
 * @brief Draws text on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color){
    if(!getAccessState()) return -1;
    return _device->drawText(text, x, y, size, bg_color, fg_color);
}

/**
 * @brief Draws a bitmap on the display
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color){
    if(!getAccessState()) return -1;
    return _device->drawBitmap(x, y, bitmap, width, height, bg_color, fg_color);
}

/**
 * @brief Fills the screen with a colored color
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::fillScreen(RGB_Color color){
    if(!getAccessState()) return -1;
    return _device->fillScreen(color);
}

//info functions
/**
 * @brief Gets the size of the text to be drawn
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height){
    if(!getAccessState()) return -1;
    return _device->getTextSize(text, x, y, size, real_x, real_y, width, height);
}

/**
 * @brief Gets the device information
 * @return int - Returns 0 on success, negative value on failure.
 */
int DisplayHandle::get_device_info(DisplayDeviceInfo_t &info){
    if(!getAccessState()) return -1;
    return _device->get_device_info(info);
}
