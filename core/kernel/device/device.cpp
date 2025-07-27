// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/device/device.h"

Device::Device(uint32_t deviceId, DeviceType deviceType) : 
_device_id(deviceId),
_deviceType(deviceType)
{
    // Constructor implementation
}

/**
 * @brief device initialization function
 * @details This is called by the kernel when the device is loaded (and starts running). This should contain hardware inizialization stuff
 */
int Device::init(DeviceInitStruct_t *init_struct){
    return 0; // Default implementation, can be overridden by derived classes
}

/**
 * @brief device de-initialization function
 * @details This is called by the kernel when the device is unloaded (stops running). This should contain hardware de-initialization stuff, memory cleaning, ecc..
 */
int Device::deinit(){
    return 0; // Default implementation, can be overridden by derived classes
}


/**
 * @brief Set the device name
 * @details It's recommended to always set the name in the class constructor
 */
void Device::setDeviceName(const std::string& name) {
    _name = name;
}

/**
 * @brief Set a device description
 * @details It's recommended to always set the description in the class constructor
 */
void Device::setDeviceDescription(const std::string& description) {
    _description = description;
}

uint32_t Device::getDeviceId() const {
    return _device_id;
}

std::string Device::getDeviceName() const { 
    return _name; 
}

std::string Device::getDeviceDescription() const { 
    return _description; 
}

DeviceType Device::getDeviceType() const {
    return _deviceType;
}