// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/device/hid_device.h"
#include "core/logging/logging.h"
#include "core/kernel/device/device_types.h"

HIDDevice::HIDDevice(uint32_t deviceId)
    : Device(deviceId, DeviceType::DEVICE_TYPE_HID)
{
    setDeviceName("HID Device");
    setDeviceDescription("Generic HID Device");
}

/**
 * @brief Update the HID device
 * @details Update the state of the devices if the device is not using interrupts for hardware state changes
 */
int HIDDevice::update(){
    return 0;
}

/**
 * @brief Get device information
 */
int HIDDevice::get_device_info(void* arg){
    return 0;
}
