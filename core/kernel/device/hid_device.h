// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef CONTROL_INPUT_DEVICE_H
#define CONTROL_INPUT_DEVICE_H

#include "core/structs/input_structs.h"
#include "core/kernel/device/device.h"
#include <string>

class HIDDeviceInitStruct_t : public DeviceInitStruct_t {
public:
    HIDDeviceInitStruct_t() {
        _deviceType = DeviceType::DEVICE_TYPE_HID; // Set the device type to
    }
    void (*input_interrupt)(InputEvent&);
};

class HIDDevice : public Device {
public:
    HIDDevice(uint32_t deviceId);
    HIDDevice() = delete;
    virtual int update();
    virtual int get_device_info(void* arg);
};

#endif // CONTROL_INPUT_DEVICE_H