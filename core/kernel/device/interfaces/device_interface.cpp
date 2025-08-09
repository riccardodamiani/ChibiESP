// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include <memory>

#include "core/kernel/device/interfaces/device_interface.h"
#include "core/kernel/components/user_handle.h"
#include "core/kernel/components/handle_state.h"

DeviceInterface::DeviceInterface(Handle handle) : _handle(std::move(handle)) {}

bool DeviceInterface::isActive() const {
    return _handle.getState() == HandleState::Active;
}

void DeviceInterface::release() {
    if (isActive()) {

    }
}