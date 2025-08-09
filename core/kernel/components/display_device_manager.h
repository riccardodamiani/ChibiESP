// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DISPLAY_DEVICE_MANAGER_H
#define DISPLAY_DEVICE_MANAGER_H

#include <cstdint>
#include <mutex>
#include <vector>
#include <memory>

#include "core/kernel/device/device_types.h"
#include "core/kernel/device/interfaces/display_handle.h"
#include "core/kernel/components/kernel_handle.h"

struct DisplayDeviceControl_t{
    DisplayDevice* device; // Pointer to the display device
    std::vector <std::unique_ptr<KernelHandle>> handles; // List of handles associated with the device
};

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

    int createHandle(DisplayId deviceId, DisplayHandle &handle);
    int freeHandle(uint32_t deviceId, uint32_t handleId);
private:
    //mutexes
    std::mutex _deviceMutex;

    //registered devices
    std::vector<DisplayDeviceControl_t> _registeredDevices;

    //handle id
    uint32_t _nextHandleId = 0; // Unique ID for the next handle
};

#endif // DISPLAY_DEVICE_MANAGER_H