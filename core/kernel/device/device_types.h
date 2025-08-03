// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef DEVICE_TYPES_H
#define DEVICE_TYPES_H

enum class DeviceType {
    DEVICE_TYPE_UNKNOWN = 0,
    DEVICE_TYPE_HID,          // Human Interface Device (e.g., buttons, wheels)
    DEVICE_TYPE_DISPLAY,      // Display devices (e.g., SSD1306)
};

typedef int DisplayId; // Type alias for display device ID

#endif // DEVICE_TYPES_H