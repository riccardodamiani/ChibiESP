// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/display_device_manager.h"
#include "core/kernel/device/display_device.h"
#include "core/kernel/device/device.h"
#include "core/logging/logging.h"

int DisplayDeviceManager::registerDevice(DisplayDevice* device) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    _registeredDevices.push_back(device);
    return 0; // Success
}

DisplayDevice* DisplayDeviceManager::getDeviceById(uint32_t deviceId) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    for (const auto& device : _registeredDevices) {
        if (device->getDeviceId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

DisplayDevice* DisplayDeviceManager::getDisplayById(DisplayId displayId) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    if(_registeredDevices.size() <= displayId) {
        return nullptr;
    }
    return _registeredDevices[displayId];
}