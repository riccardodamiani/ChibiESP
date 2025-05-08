#include "core/example_drivers/button.h"
#include "core/logging/logging.h"
#include "core/structs/input_structs.h"
#include "core/kernel/driver/control_input_driver.h"

#include <stdint.h>
#include <memory>

int CESP_ButtonDriver::configure(void* arg){
    CESP_ButtonDriverConfigStruct* config_struct = (CESP_ButtonDriverConfigStruct*)arg;
    if (config_struct == nullptr) {
        return 0; // Nothing to configure
    }

    //checks for id confict between devices
    for(int dev_index_1 = 0; dev_index_1 < config_struct->devices.size(); ++dev_index_1) {
        for(int dev_index_2 = dev_index_1 + 1; dev_index_2 < config_struct->devices.size(); ++dev_index_2) {
            if(config_struct->devices[dev_index_1].deviceID == config_struct->devices[dev_index_2].deviceID) {
                Logger::error("Button Driver: Device ID conflict between devices %d and %d", dev_index_1, dev_index_2);
                Logger::error("Button Driver: leaving unconfigured");
                return -1; // Error: device ID conflict: leave unconfigured
            }
        }
    }

    //Initialize internal the structure for each device
    for(const auto& device : config_struct->devices) {
        std::unique_ptr<InternalDeviceInfo> internal_device_state(new InternalDeviceInfo());
        internal_device_state->deviceID = device.deviceID;
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
int CESP_ButtonDriver::init(ControlDriverInitStruct_t& init_struct){

    _input_interrupt = init_struct.input_interrupt;
    if (_input_interrupt == nullptr) {
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
            struct InputEvent event;
            event.deviceID = device->deviceID;
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

int CESP_ButtonDriver::deinit(){
    return 0; // Success
}


int CESP_ButtonDriver::update(){

    //update device state
    for(auto& device : _devices) {
        update_device_state(device);
    }
    return 0;
}


int CESP_ButtonDriver::get_device_info(void* arg){
    return 0;
}