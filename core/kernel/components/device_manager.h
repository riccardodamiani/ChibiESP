// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <vector>
#include <mutex>

#include "core/structs/input_structs.h"

class HIDDevice;
class DisplayDevice;
class DisplayHandle;

struct HIDControlStruct_t{
    HIDDevice* device;
};

struct DisplayControlStruct_t{
    DisplayDevice* device;
};

class DeviceManager {
public:
    int register_hid_device(HIDDevice* device);
    int register_display_device(DisplayDevice* device);

    void init_hid_devices(void input_interrupt_callback(InputEvent &event));
    void init_display_devices();

    //control input device functions
    int update_hid_devices_state();

    //display device functions
    DisplayDevice*  get_display_device_by_id(uint32_t deviceId);
private:
    //mutexes 
    std::mutex _displayDeviceMutex;
    std::mutex _controlInputDeviceMutex;

    //registered devices
    std::vector <HIDControlStruct_t> _regHIDDevices; // List of input devices registered
    std::vector <DisplayControlStruct_t> _regDisplayDevices; // List of devices registered
};

#endif // DEVICE_MANAGER_H