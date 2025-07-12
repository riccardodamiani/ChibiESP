// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include <string>
#include <atomic>
#include <memory>

#include "core/kernel/device/control_input_device.h"

struct ButtonDeviceConfigStruct{
    uint8_t gpio_pin; // GPIO pin for button
    bool normally_open; // normally opened or closed?
    uint32_t debounce_time_ms; // Debounce time in milliseconds
};

class ButtonDevice : public ControlInputDevice{
public:
    ButtonDevice(uint32_t deviceId);
    int configure(ButtonDeviceConfigStruct config);
    int init(ControlDeviceInitStruct_t& init_struct) override;
    int deinit() override;
    int update() override;
    int get_device_info(void* arg) override;

private:
struct InternalDeviceInfo{
    uint8_t gpio_pin;
    uint8_t normally_open; // normally opened or closed?
    uint32_t debounce_time_ms; // Debounce time in milliseconds
    std::atomic <uint32_t> last_change_time_ms; // Last time the button state changed
    std::atomic <uint8_t> last_state; // Last state of the button (pressed or not pressed)
};

    void update_device_state();

    InternalDeviceInfo *_device;
    void (*_input_interrupt)(InputEvent&); // Callback function for button events
};

#endif //BUTTON_H