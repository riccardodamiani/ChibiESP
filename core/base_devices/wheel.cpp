// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/base_devices/wheel.h"
#include "core/logging/logging.h"
#include "core/structs/input_structs.h"
#include "core/kernel/device/control_input_device.h"

#include <EncoderStepCounter.h>
#include <memory>

std::vector<WheelDevice*> WheelDevice::_instances; // Static vector to hold all instances of WheelDevice

WheelDevice::WheelDevice(uint32_t deviceId) : 
    ControlInputDevice(deviceId)
{
    _device = nullptr;
    _input_interrupt = nullptr;
}

int WheelDevice::configure(WheelDeviceConfigStruct config){
    //Initialize internal the structure for the _device
    _device = new InternalDeviceInfo();
    _device->s1_gpio = config.s1_gpio;
    _device->s2_gpio = config.s2_gpio;
    _device->invert = config.invert ? 1 : 0;
    _device->debounce_time_ms = config.debounce_time_ms;
    _device->last_change_time_ms = 0;
    _device->encoder = nullptr;

    _instances.push_back(this); // Add this instance to the static vector

    return 0;
}

// ISR to update all wheel devices
void WheelDevice::update_isr(){
    for(WheelDevice* device : _instances) {
        device->device_update_isr();
    }
}

// Update the encoder state for this specific device
void WheelDevice::device_update_isr(){
    if(_device && _device->encoder != nullptr) {
        _device->encoder->tick(); // Update the encoder state
    }
}

//initialize the encoders and attach interrupts
int WheelDevice::init(ControlDeviceInitStruct_t& init_struct){
    if(_device == nullptr) {
        Logger::error("Wheel Device: Init error: no device available");
        return -1; // Nothing to initialize
    }

    if(_instances.size() >= MAX_WHEEL_DEVICES) {
        Logger::error("Wheel Device: Configuration error: maximum number of wheel devices reached (%d)", MAX_WHEEL_DEVICES);
        return -1; // Maximum number of devices reached
    }

    _input_interrupt = init_struct.input_interrupt; // Set the callback function for input manager
    if (_input_interrupt == nullptr) {
        Logger::error("Wheel _device: Init error: no callback function provided for input manager");
        _instances.pop_back(); // Remove this instance from the static vector
        return -1; // Nothing to initialize
    }

    //initialize the GPIO pin for the _device
    _device->encoder = new EncoderStepCounter(_device->s1_gpio, _device->s2_gpio);
    if(_device->encoder == nullptr) {
        Logger::error("Wheel _device: Init error: unable to create encoder object for device %d", _device->deviceId);
        _instances.pop_back(); // Remove this instance from the static vector
        return -1; // Skip this _device
    }

    //create interrupts from the GPIO pins
    _device->interrupt_s1 = digitalPinToInterrupt(_device->s1_gpio);
    _device->interrupt_s2 = digitalPinToInterrupt(_device->s2_gpio);
    if(_device->interrupt_s1 == NOT_AN_INTERRUPT || _device->interrupt_s2 == NOT_AN_INTERRUPT) {
        Logger::error("Wheel _device: Init error: unable to create interrupt for device %d", _device->deviceId);
        delete _device->encoder;
        _device->encoder = nullptr; // Clean up the encoder object
        _instances.pop_back(); // Remove this instance from the static vector
        return -1;
    }

    //update the encoder state the first time
    _device->encoder->begin();
    _device->encoder->tick();
    _device->last_change_time_ms = millis();

    //attach interrupts for the encoder pins
    attachInterrupt(_device->interrupt_s1, WheelDevice::update_isr, CHANGE);
    attachInterrupt(_device->interrupt_s2, WheelDevice::update_isr, CHANGE);
    return 0;
}

void WheelDevice::update_device_state(){

    if(!_device || _device->encoder == nullptr) {
        return; // Skip this _device if encoder is not initialized
    }

    //update encoder state
    int delta = _device->encoder->getPosition();
    if(delta != 0) {
        uint32_t now = millis();
        if(now - _device->last_change_time_ms > _device->debounce_time_ms) {
            
            delta = _device->invert ? -delta : delta; // Invert the delta if needed

            //update state and time
            _device->last_change_time_ms = now;
            _device->encoder->reset();

            //call the interrupt callback function
            struct InputEvent event;
            event.deviceID = get_device_id();
            event.type = InputEventType::INPUT_EVENT_WHEEL;
            event.deviceEventType = 0; // TODO: define the event type for wheel events
            event.eventData = delta; // Store the delta in the event data

            _input_interrupt(event);
        }
    }
}

/// Deinitialize the encoders and detach interrupts
int WheelDevice::deinit(){

    if(_device && _device->encoder != nullptr) {
        detachInterrupt(_device->interrupt_s1);
        detachInterrupt(_device->interrupt_s2);
    }
    delay(10); // waits any interrupt call to finish
    if(_device->encoder != nullptr) {
        delete _device->encoder; // Clean up the encoder object
        _device->encoder = nullptr;
    }
    return 0; // Success
}

//update device state
int WheelDevice::update(){
    update_device_state();
    return 0;

}

int WheelDevice::get_device_info(void* arg){
    return 0;
}