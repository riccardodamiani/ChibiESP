// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DISPLAY_HANDLE_H
#define DISPLAY_HANDLE_H

#include "core/kernel/components/user_handle.h"
#include "core/kernel/device/display_device.h"

#include <memory>

class DisplayHandle{
public:
    DisplayHandle();
    DisplayHandle(DisplayDevice *device, std::unique_ptr<Handle> handle);
    ~DisplayHandle();
    
    // Delete copy constructor and copy assignment operator
    DisplayHandle(const DisplayHandle&) = delete;
    DisplayHandle& operator=(const DisplayHandle&) = delete;

    // Move constructor
    DisplayHandle(DisplayHandle&& other) noexcept
        : _handle(std::move(other._handle)), _device(other._device) {
        other._device = nullptr;
    }

    // Move assignment operator
    DisplayHandle& operator=(DisplayHandle&& other) noexcept {
        if (this != &other) {
            _handle = std::move(other._handle);
            _device = other._device;
            other._device = nullptr;
        }
        return *this;
    }

    void close();

    //handle access methods
    bool getAccessState() const;

    /**
     * DISPLAY DEVICE INTERFACE
     * @brief These functions are used to interact with the display device.
     */
    int updateScreen();
    int clearScreen();

    //functions for monochromatic displays
    int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color);
    int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color);
    int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color);
    int drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color);
    int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color);
    int fillScreen(BW_Color color);

    //functions for colored displays
    int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color);
    int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color);
    int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color);
    int drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color);
    int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color);
    int fillScreen(RGB_Color color);

    //info functions
    int getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height);
    int get_device_info(DisplayDeviceInfo_t &info);
private:
    std::unique_ptr<Handle> _handle;
    DisplayDevice* _device{nullptr}; // Pointer to the associated display device
};


#endif // DISPLAY_HANDLE_H