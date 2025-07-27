// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/base_devices/button.h"
#include "core/logging/logging.h"
#include "core/structs/input_structs.h"
#include "core/kernel/device/hid_device.h"
#include "core/kernel/device/device.h"

#include <stdint.h>
#include <memory>

ButtonDevice::ButtonDevice(uint32_t deviceId):
    HIDDevice(deviceId)
{
    setDeviceName("Button Device");
    setDeviceDescription("Generic button device");
    _device = nullptr;
    _input_interrupt = nullptr;
}

int ButtonDevice::configure(ButtonDeviceConfigStruct config){

    //Initialize internal the structure for the device
    _device = new InternalDeviceInfo();
    _device->gpio_pin = config.gpio_pin;
    _device->normally_open = config.normally_open ? 1 : 0;
    _device->debounce_time_ms = config.debounce_time_ms;
    _device->last_change_time_ms = 0;
    _device->last_state = 0;

    return 0;
}


//initialize the gpio for each button
int ButtonDevice::init(DeviceInitStruct_t* init_struct){

    if(init_struct->getDeviceType() != DeviceType::DEVICE_TYPE_HID) {
        Logger::error("Button Device: Init error: init_struct is not of type HIDDeviceInitStruct_t");
        return -1; // Invalid init structure
    }

    HIDDeviceInitStruct_t* hid_init_struct = static_cast<HIDDeviceInitStruct_t*>(init_struct);

    _input_interrupt = hid_init_struct->input_interrupt;
    if (_input_interrupt == nullptr) {
        if(_device != nullptr){
            delete _device;
            _device = nullptr;
        }
        Logger::error("Button Device: Init error: no callback function provided for input manager");
        return -2; // Nothing to configure
    }

    //initialize the GPIO pin for each device
    pinMode(_device->gpio_pin, INPUT_PULLUP);
    uint8_t reading = digitalRead(_device->gpio_pin);
    uint8_t new_state = (reading ^ _device->normally_open)&1;
    _device->last_state = new_state;
    _device->last_change_time_ms = millis();
    return 0;
}

void ButtonDevice::update_device_state(){
    uint8_t reading = digitalRead(_device->gpio_pin);
    uint8_t new_state = (reading ^ _device->normally_open)&1;
    if(new_state != _device->last_state) {
        uint32_t current_time = millis();
        if(current_time - _device->last_change_time_ms > _device->debounce_time_ms) {
            // State has changed and debounce time has passed
            _device->last_change_time_ms = current_time;
            _device->last_state = new_state;
            struct InputEvent event;
            event.deviceID = getDeviceId();
            event.type = InputEventType::INPUT_EVENT_KEY; //button events are of type KEY
            event.eventData = 0;
            if(new_state) {
                event.deviceEventType = static_cast<uint8_t>(KeyEventType::KEY_EVENT_PRESSED); // Button pressed
            } else {
                event.deviceEventType = static_cast<uint8_t>(KeyEventType::KEY_EVENT_RELEASED); // Button released
            }

            // Call the callback function with the button ID and new state
            _input_interrupt(event);
        }
    }
}

int ButtonDevice::deinit(){
    return 0; // Success
}


int ButtonDevice::update(){

    //update device state
    update_device_state();
    return 0;
}


int ButtonDevice::get_device_info(void* arg){
    return 0;
}