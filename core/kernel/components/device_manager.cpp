// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/device_manager.h"
#include "core/kernel/device/control_input_device.h"
#include "core/kernel/device/display_device.h"
#include "core/logging/logging.h"

#include <mutex>

int DeviceManager::register_control_input_device(ControlInputDevice* device){
    std::lock_guard<std::mutex> lock(_controlInputDeviceMutex); // Lock the mutex for thread safety
    for (const auto& existing_device : _regControlInputDevices) {
        if (existing_device.device->get_device_id() == device->get_device_id()) {
            Logger::error("Input device %d is already registered", device->get_device_id());
            return -1; // Device already registered
        }
    }

    // Register the new device
    ControlInputControlStruct_t device_struct;
    device_struct.device = device;
    _regControlInputDevices.push_back(device_struct);
    Logger::info("Input device %d registered", device->get_device_id());
    return 0; // Success
}

int DeviceManager::register_display_device(DisplayDevice* device){
    std::lock_guard<std::mutex> lock(_displayDeviceMutex); // Lock the mutex for thread safety
    for (const auto& existing_device : _regDisplayDevices) {
        if (existing_device.device->get_device_id() == device->get_device_id()) {
            Logger::error("Display device %d is already registered", device->get_device_id());
            return -1; // Device already registered
        }
    }

    // Register the new device
    DisplayControlStruct_t device_struct;
    device_struct.device = device;
    _regDisplayDevices.push_back(device_struct);
    Logger::info("Display device %d registered", device->get_device_id());
    return 0; // Success
}

void DeviceManager::init_control_input_devices(void input_interrupt_callback(InputEvent &)){
    // Initialize control input devices
    for (auto& inputDevice : _regControlInputDevices) {
        ControlDeviceInitStruct_t init_struct;
        init_struct.input_interrupt = input_interrupt_callback; // Set the input interrupt callback function
        if(inputDevice.device->init(init_struct) < 0){
            Logger::error("Failed to initialize control input device %d", inputDevice.device->get_device_id());
        }
    }
}

void DeviceManager::init_display_devices(){
    // Initialize display devices
    for (auto& displayDevice : _regDisplayDevices) {
        if(displayDevice.device->init() < 0){
            Logger::error("Failed to initialize display device %d", displayDevice.device->get_device_id());
        }
    }
}

int DeviceManager::update_control_input_devices_state(){
    std::lock_guard<std::mutex> lock(_controlInputDeviceMutex); // Lock the mutex for thread safety
    for (auto& inputDevice : _regControlInputDevices) {
        if(inputDevice.device->update() < 0){
            //Logger::error("Failed to update control input device %s", inputDevice.device->get_name().c_str());
        }
    }
    return 0; // Success
}

DisplayDevice* DeviceManager::get_display_device_by_id(uint32_t deviceId){
    std::lock_guard<std::mutex> lock(_displayDeviceMutex); // Lock the mutex for thread safety
    for (auto& displayDevice : _regDisplayDevices) {
        if(displayDevice.device->get_device_id() == deviceId){
            return displayDevice.device; // Return the device if found
        }
    }
    return nullptr; // Device not found
}