// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/device_manager.h"
#include "core/kernel/device/device.h"
#include "core/kernel/device/hid_device.h"
#include "core/kernel/device/display_device.h"
#include "core/logging/logging.h"

#include <mutex>

int DeviceManager::register_control_input_device(HIDDevice* device){
    std::lock_guard<std::mutex> lock(_controlInputDeviceMutex); // Lock the mutex for thread safety
    for (const auto& existing_device : _regHIDDevices) {
        if (existing_device.device->getDeviceId() == device->getDeviceId()) {
            Logger::error("Input device %d is already registered", device->getDeviceId());
            return -1; // Device already registered
        }
    }

    // Register the new device
    ControlInputControlStruct_t device_struct;
    device_struct.device = device;
    _regHIDDevices.push_back(device_struct);
    Logger::info("Input device %d registered", device->getDeviceId());
    return 0; // Success
}

int DeviceManager::register_display_device(DisplayDevice* device){
    std::lock_guard<std::mutex> lock(_displayDeviceMutex); // Lock the mutex for thread safety
    for (const auto& existing_device : _regDisplayDevices) {
        if (existing_device.device->getDeviceId() == device->getDeviceId()) {
            Logger::error("Display device %d is already registered", device->getDeviceId());
            return -1; // Device already registered
        }
    }

    // Register the new device
    DisplayControlStruct_t device_struct;
    device_struct.device = device;
    _regDisplayDevices.push_back(device_struct);
    Logger::info("Display device %d registered", device->getDeviceId());
    return 0; // Success
}

void DeviceManager::init_control_input_devices(void input_interrupt_callback(InputEvent &)){
    // Initialize control input devices
    HIDDeviceInitStruct_t *init_struct = new HIDDeviceInitStruct_t();
    init_struct->input_interrupt = input_interrupt_callback; // Set the input interrupt callback function
    for (auto& inputDevice : _regHIDDevices) {
        if(inputDevice.device->init(init_struct) < 0){
            Logger::error("Failed to initialize control input device %d", inputDevice.device->getDeviceId());
        }else{
            Logger::info("Control input device %d initialized", inputDevice.device->getDeviceId());
        }
    }
    delete init_struct;
}

void DeviceManager::init_display_devices(){
    // Initialize display devices
    for (auto& displayDevice : _regDisplayDevices) {
        if(displayDevice.device->init() < 0){
            Logger::error("Failed to initialize display device %d", displayDevice.device->getDeviceId());
        }else{
            Logger::info("Display device %d initialized", displayDevice.device->getDeviceId());
        }
    }
}

int DeviceManager::update_control_input_devices_state(){
    std::lock_guard<std::mutex> lock(_controlInputDeviceMutex); // Lock the mutex for thread safety
    for (auto& inputDevice : _regHIDDevices) {
        if(inputDevice.device->update() < 0){
            //Logger::error("Failed to update control input device %s", inputDevice.device->get_name().c_str());
        }
    }
    return 0; // Success
}

DisplayDevice* DeviceManager::get_display_device_by_id(uint32_t deviceId){
    std::lock_guard<std::mutex> lock(_displayDeviceMutex); // Lock the mutex for thread safety
    for (auto& displayDevice : _regDisplayDevices) {
        if(displayDevice.device->getDeviceId() == deviceId){
            return displayDevice.device; // Return the device if found
        }
    }
    return nullptr; // Device not found
}