// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DISPLAY_DEVICE_MANAGER_H
#define DISPLAY_DEVICE_MANAGER_H

#include <cstdint>
#include <mutex>
#include <vector>

#include "core/kernel/device/device_types.h"

class DisplayDevice;

/**
 * Manages display devices.
 * Only the devices already initialized are registered here
 */
class DisplayDeviceManager {
public:
    int registerDevice(DisplayDevice* device);
    DisplayDevice* getDeviceById(uint32_t deviceId);
    DisplayDevice* getDisplayById(DisplayId displayId);
private:
    //mutexes
    std::mutex _deviceMutex;

    //registered devices
    std::vector<DisplayDevice*> _registeredDevices;
};

#endif // DISPLAY_DEVICE_MANAGER_H