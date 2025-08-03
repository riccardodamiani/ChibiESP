// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/hid_device_manager.h"
#include "core/kernel/device/hid_device.h"
#include "core/kernel/device/device.h"
#include "core/logging/logging.h"

#include <mutex>
#include <vector>

HIDDevice* HIDDeviceManager::getDeviceById(uint32_t deviceId){
    std::lock_guard<std::mutex> lock(_deviceMutex);
    for (const auto& device : _registeredDevices) {
        if (device->getDeviceId() == deviceId) {
            return device;
        }
    }
    return nullptr;
}

int HIDDeviceManager::updateDeviceState(){
    std::lock_guard<std::mutex> lock(_deviceMutex); // Lock the mutex for thread safety
    for (auto& inputDevice : _registeredDevices) {
        if(inputDevice->update() < 0){
            //Logger::error("Failed to update control input device %s", inputDevice->get_name().c_str());
        }
    }
    return 0; // Success
}

int HIDDeviceManager::registerDevice(HIDDevice* device){
    std::lock_guard<std::mutex> lock(_deviceMutex);
    _registeredDevices.push_back(device);
    return 0; // Success
}