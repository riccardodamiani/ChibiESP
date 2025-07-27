// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DEVICE_H
#define DEVICE_H

#include <string>
#include <stdint.h>

#include "core/kernel/device/device_types.h"

class DeviceInitStruct_t{
public:
    virtual ~DeviceInitStruct_t() = default; // Virtual destructor for proper cleanup

    DeviceType getDeviceType() const {
        return _deviceType;
    }
protected:
    DeviceType _deviceType{DeviceType::DEVICE_TYPE_UNKNOWN}; // Type of the device
};

class Device{
public:
    Device(uint32_t deviceId, DeviceType deviceType);
    Device() = delete; // Prevent default constructor
    virtual ~Device() = default;
    virtual int init(DeviceInitStruct_t *init_struct = nullptr);
    virtual int deinit();

    uint32_t getDeviceId() const;
    std::string getDeviceName() const;
    std::string getDeviceDescription() const;
    DeviceType getDeviceType() const;

    // Not copiable nor movable
    Device(const Device&) = delete;
    Device& operator=(const Device&) = delete;
    Device(Device&&) = delete;
    Device& operator=(Device&&) = delete;
protected:
    void setDeviceName(const std::string& name);
    void setDeviceDescription(const std::string& description);
private:
    DeviceType _deviceType{DeviceType::DEVICE_TYPE_UNKNOWN}; // Type of the device
    uint32_t _device_id; // Unique identifier for the device
    std::string _name; // Name of the device
    std::string _description; // Description of the device
};

#endif // DEVICE_H