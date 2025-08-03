// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <vector>
#include <mutex>

#include "core/structs/input_structs.h"
#include "core/kernel/components/display_device_manager.h"
#include "core/kernel/components/hid_device_manager.h"
#include "core/kernel/device/device_types.h"

class Device;
class HIDDevice;
class DisplayDevice;

class DeviceManager {
public:
    int register_device(Device* device);

    void init_devices(void input_interrupt_callback(InputEvent &));
    int initHidDevice(HIDDevice* device, void input_interrupt_callback(InputEvent &));
    int initDisplayDevice(DisplayDevice* device);

    //control input device functions
    int updateHidDevicesState();

    Device* getDeviceById(uint32_t deviceId);

    //display device functions
    DisplayDevice* getDisplayDeviceById(DisplayId displayId);
private:
    //mutexes
    std::mutex _deviceMutex;

    //registered devices
    std::vector <Device*> _devices;

    //device managers
    HIDDeviceManager _hidDeviceManager;
    DisplayDeviceManager _displayDeviceManager;
};

#endif // DEVICE_MANAGER_H