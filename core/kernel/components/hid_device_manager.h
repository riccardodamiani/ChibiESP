// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef HID_DEVICE_MANAGER_H
#define HID_DEVICE_MANAGER_H

#include <cstdint>
#include <mutex>
#include <vector>

#include "core/structs/input_structs.h"

class HIDDevice;

/**
 * Manages HID devices.
 * Only the devices already initialized are registered here
 */
class HIDDeviceManager{
public:
    int registerDevice(HIDDevice* device);
    int updateDeviceState();
    HIDDevice* getDeviceById(uint32_t deviceId);
private:
    //mutexes
    std::mutex _deviceMutex;

    //registered devices
    std::vector<HIDDevice*> _registeredDevices;
};

#endif // HID_DEVICE_MANAGER_H