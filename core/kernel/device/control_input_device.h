// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef CONTROL_INPUT_DEVICE_H
#define CONTROL_INPUT_DEVICE_H

#include "core/structs/input_structs.h"
#include <string>

struct ControlDeviceInitStruct_t{
    void (*input_interrupt)(InputEvent&);
};

class ControlInputDevice {
public:
    ControlInputDevice(uint32_t deviceId);
    ControlInputDevice() = delete;
    virtual int init(ControlDeviceInitStruct_t& init_struct);
    virtual int deinit();
    virtual int update();
    virtual int get_device_info(void* arg);
    uint32_t get_device_id() const { return _deviceId; }
private:
    uint32_t _deviceId; // Device ID
};

#endif // CONTROL_INPUT_DEVICE_H