// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/display_device_manager.h"
#include "core/kernel/device/display_device.h"
#include "core/kernel/device/device.h"
#include "core/logging/logging.h"
#include "core/kernel/components/handle_state.h"
#include "core/kernel/components/kernel_handle.h"
#include "core/kernel/components/user_handle.h"

int DisplayDeviceManager::registerDevice(DisplayDevice* device) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    DisplayDeviceControl_t deviceInfo;
    deviceInfo.device = device; // Set the device pointer
    deviceInfo.handles.clear(); // Clear any existing handles for this device
    _registeredDevices.push_back(std::move(deviceInfo));
    return 0; // Success
}

DisplayDevice* DisplayDeviceManager::getDeviceById(uint32_t deviceId) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    for (const auto& deviceInfo : _registeredDevices) {
        if (deviceInfo.device->getDeviceId() == deviceId) {
            return deviceInfo.device;
        }
    }
    return nullptr;
}

DisplayDevice* DisplayDeviceManager::getDisplayById(DisplayId displayId) {
    std::lock_guard<std::mutex> lock(_deviceMutex);
    if(_registeredDevices.size() <= displayId) {
        return nullptr;
    }
    return _registeredDevices[displayId].device;
}

int DisplayDeviceManager::createHandle(DisplayId displayId, DisplayHandle &displayHandle){
    DisplayDevice* device = getDisplayById(displayId);
    if (!device) {
        return -1; // Error: device not found
    }

    //create the handles
    std::shared_ptr<HandleStateWrapper> handleState(new HandleStateWrapper(HandleState::Idle));
    std::unique_ptr<KernelHandle> kernelHandle(new KernelHandle(handleState, _nextHandleId));
    std::unique_ptr<Handle> handle(new Handle(handleState, _nextHandleId));
    _nextHandleId++; // Increment the handle ID for the next handle

    DisplayHandle outHandle(device, std::move(handle));
    displayHandle = std::move(outHandle); // Move the handle to the output parameter

    //remove access to the previous handle if it exists
    std::lock_guard<std::mutex> lock(_deviceMutex);
    if( _registeredDevices[displayId].handles.size() > 0){
        _registeredDevices[displayId].handles.back()->setState(HandleState::Suspended);
    }

    kernelHandle->setState(HandleState::Active); // Set the handle state to Active
    _registeredDevices[displayId].handles.push_back(std::move(kernelHandle)); // Add the handle to the device's handles list

    return 0; // Success
}

int DisplayDeviceManager::freeHandle(uint32_t deviceId, uint32_t handleId){
    std::lock_guard<std::mutex> lock(_deviceMutex);
    for (auto& deviceInfo : _registeredDevices) {
        if (deviceInfo.device->getDeviceId() == deviceId) {
            for (auto it = deviceInfo.handles.begin(); it != deviceInfo.handles.end(); ++it) {
                if ((*it)->getId() == handleId) {
                    Logger::info("Freeing handle %d for device %d", handleId, deviceId);
                    bool activateNextOne = false;
                    //if it's the current active handle
                    if((*it)->getState() == HandleState::Active && deviceInfo.handles.size() > 1) {
                        activateNextOne = true;
                    }
                    (*it)->setState(HandleState::Released);
                    deviceInfo.handles.erase(it);
                    if (activateNextOne) {
                        --it; // Adjust iterator
                        deviceInfo.handles.back()->setState(HandleState::Active);
                        Logger::info("Activating next handle %d for device %d", deviceInfo.handles.back()->getId(), deviceId);
                    }
                    return 0; // Success
                }
            }
        }
    }
    return -1; // Error: handle not found
}