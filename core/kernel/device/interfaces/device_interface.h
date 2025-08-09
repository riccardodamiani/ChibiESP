// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DEVICE_INTERFACE_H
#define DEVICE_INTERFACE_H

#include <memory>

#include "core/kernel/components/user_handle.h"
#include "core/kernel/device/device.h"

class DeviceInterface{
public:
    DeviceInterface(Handle handle);
    virtual ~DeviceInterface() = default;

    bool isActive() const;
    void release();
private:
    Handle _handle;
    Device *_device{nullptr};
};

#endif // DEVICE_INTERFACE_H