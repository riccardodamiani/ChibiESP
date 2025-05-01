#include "driver/button.h"
#include "core/logging.h"

#include <stdint.h>
#include <memory>

int CESP_ButtonDriver::configure(void* arg){
    CESP_ButtonDriverConfigStruct* config_struct = (CESP_ButtonDriverConfigStruct*)arg;
    if (config_struct == nullptr) {
        return 0; // Nothing to configure
    }

    //Initialize internal the structure for each device
    for(const auto& device : config_struct->devices) {
        std::unique_ptr<InternalDeviceInfo> internal_device_state(new InternalDeviceInfo());
        internal_device_state->buttonID = device.buttonID;
        internal_device_state->gpio_pin = device.gpio_pin;
        internal_device_state->normally_open = device.normally_open ? 1 : 0;
        internal_device_state->debounce_time_ms = device.debounce_time_ms;
        internal_device_state->last_change_time_ms = 0;
        internal_device_state->last_state = 0;
        _devices.push_back(std::move(internal_device_state)); // Store the unique_ptr in the vector
    }

    _name = "ButtonDriver";

    return 0;
}


//initialize the gpio for each button
int CESP_ButtonDriver::init(void* arg){

    CESP_ButtonDriverInitStruct* init_struct = (CESP_ButtonDriverInitStruct*)arg;
    if (init_struct == nullptr) {
        _devices.clear(); // Clear the devices if no callback is provided to avoid calling the callback
        Logger::error("Button Driver: Init error: args was nullptr");
        return -2; // Nothing to configure
    }

    _input_manager_callback = init_struct->callback;
    if (_input_manager_callback == nullptr) {
        _devices.clear(); // Clear the devices if no callback is provided to avoid calling the callback
        Logger::error("Button Driver: Init error: no callback function provided for input manager");
        return -2; // Nothing to configure
    }

    //initialize the GPIO pin for each device
    for(auto& device : _devices) {
        pinMode(device->gpio_pin, INPUT_PULLUP);
        uint8_t reading = digitalRead(device->gpio_pin);
        uint8_t new_state = (reading ^ device->normally_open)&1;
        device->last_state = new_state;
        device->last_change_time_ms = millis();
    }
    return 0;
}

void CESP_ButtonDriver::update_device_state(const std::unique_ptr<InternalDeviceInfo>& device){
    uint8_t reading = digitalRead(device->gpio_pin);
    uint8_t new_state = (reading ^ device->normally_open)&1;
    if(new_state != device->last_state) {
        uint32_t current_time = millis();
        if(current_time - device->last_change_time_ms > device->debounce_time_ms) {
            // State has changed and debounce time has passed
            device->last_change_time_ms = current_time;
            device->last_state = new_state;
            // Call the callback function with the button ID and new state
            _input_manager_callback(device->buttonID, new_state);
        }
    }
}

int CESP_ButtonDriver::deinit(void* arg){
    return 0; // Success
}


int CESP_ButtonDriver::update(void* arg){

    //update device state
    for(auto& device : _devices) {
        update_device_state(device);
    }
    return 0;
}
