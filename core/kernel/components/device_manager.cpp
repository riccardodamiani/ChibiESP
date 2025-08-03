// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/device_manager.h"
#include "core/kernel/device/device.h"
#include "core/kernel/device/hid_device.h"
#include "core/kernel/device/display_device.h"
#include "core/logging/logging.h"

#include <mutex>

int DeviceManager::register_device(Device* device){
    if(device == nullptr) {
        return -1; // Error: null device
    }

    if(getDeviceById(device->getDeviceId()) != nullptr) {
        Logger::error("Device %d is already registered", device->getDeviceId());
        return -1; // Error: device already registered
    }

    std::lock_guard<std::mutex> lock(_deviceMutex); // Lock the mutex for thread safety
    _devices.push_back(device); // Add the device to the list of registered devices
    Logger::info("Device %d registered. (name=%s, type=%d)", device->getDeviceId(), device->getDeviceName().c_str(), device->getDeviceType());
    return 0; // Success
}

void DeviceManager::init_devices(void input_interrupt_callback(InputEvent &)){
    // Initialize all registered devices
    std::lock_guard<std::mutex> lock(_deviceMutex); // Lock the mutex for thread safety
    for (auto& device : _devices) {
        int result = -1;
        switch(device->getDeviceType()) {
            case DeviceType::DEVICE_TYPE_HID:
                result = initHidDevice(static_cast<HIDDevice*>(device), input_interrupt_callback);
                break;
            case DeviceType::DEVICE_TYPE_DISPLAY:
                result = initDisplayDevice(static_cast<DisplayDevice*>(device));
                break;
            default:
                Logger::warning("Unknown device type for device %d", device->getDeviceId());
        }

        if (result < 0) {
            Logger::error("Failed to initialize device %d (%s)", device->getDeviceId(), device->getDeviceName().c_str());
        } else {
            Logger::info("Device %d initialized (%s)", device->getDeviceId(), device->getDeviceName().c_str());
        }
    }
}

int DeviceManager::initHidDevice(HIDDevice* device, void input_interrupt_callback(InputEvent &)){
    HIDDeviceInitStruct_t *init_struct = new HIDDeviceInitStruct_t();
    init_struct->input_interrupt = input_interrupt_callback; // Set the input interrupt callback function

    if (device->init(init_struct) < 0) {
        Logger::error("Failed to initialize HID device %d (%s)", device->getDeviceId(), device->getDeviceName().c_str());
        return -1;
    }

    _hidDeviceManager.registerDevice(device);
    delete init_struct;
    return 0;
}

int DeviceManager::initDisplayDevice(DisplayDevice* device){
    if (device->init() < 0) {
        Logger::error("Failed to initialize display device %d (%s)", device->getDeviceId(), device->getDeviceName().c_str());
        return -1;
    }

    _displayDeviceManager.registerDevice(device);
    return 0; // Success
}

Device* DeviceManager::getDeviceById(uint32_t deviceId){
    std::lock_guard<std::mutex> lock(_deviceMutex); // Lock the mutex for thread safety
    for (const auto& device : _devices) {
        if (device->getDeviceId() == deviceId) {
            return device; // Return the device if found
        }
    }
    return nullptr; // Device not found
}

int DeviceManager::updateHidDevicesState(){
    return _hidDeviceManager.updateDeviceState(); // Delegate to HIDDeviceManager
}

DisplayDevice* DeviceManager::getDisplayDeviceById(DisplayId displayId){
    return _displayDeviceManager.getDisplayById(displayId); // Delegate to DisplayDeviceManager
}