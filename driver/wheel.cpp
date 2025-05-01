#include "driver/wheel.h"
#include "core/logging.h"

#include <EncoderStepCounter.h>
#include <memory>

std::vector<std::unique_ptr<CESP_WheelDriver::InternalDeviceInfo>> CESP_WheelDriver::_devices;

int CESP_WheelDriver::configure(void* arg){
    CESP_WheelDriverConfigStruct* config_struct = (CESP_WheelDriverConfigStruct*)arg;
    if (config_struct == nullptr) {
        return 0; // Nothing to configure
    }

    //Initialize internal the structure for each device
    for(const auto& device : config_struct->devices) {
        std::unique_ptr<InternalDeviceInfo> internal_device_state(new InternalDeviceInfo());
        internal_device_state->deviceId = device.deviceID;
        internal_device_state->s1_gpio = device.s1_gpio;
        internal_device_state->s2_gpio = device.s2_gpio;
        internal_device_state->invert = device.invert ? 1 : 0;
        internal_device_state->debounce_time_ms = device.debounce_time_ms;
        internal_device_state->last_change_time_ms = 0;
        internal_device_state->encoder = nullptr;
        CESP_WheelDriver::_devices.push_back(std::move(internal_device_state)); // Store the unique_ptr in the vector
    }

    _name = "WheelDriver";

    return 0;
}

// ISR to update the encoder state
// TODO: avoid updating all devices, only the one that triggered the interrupt
void CESP_WheelDriver::update_device_isr(){
    for(auto& device : CESP_WheelDriver::_devices) {
        if(device->encoder != nullptr) {
            device->encoder->tick(); // Update the encoder state
        }
    }
}

//initialize the encoders and attach interrupts
int CESP_WheelDriver::init(void* arg){

    CESP_WheelDriverInitStruct* init_struct = (CESP_WheelDriverInitStruct*)arg;
    if (init_struct == nullptr) {
        CESP_WheelDriver::_devices.clear(); // Clear the devices if no callback is provided to avoid calling the callback
        Logger::error("Wheel Driver: Init error: args was nullptr");
        return -2; // Nothing to configure
    }

    _input_manager_callback = init_struct->callback;
    if (_input_manager_callback == nullptr) {
        CESP_WheelDriver::_devices.clear(); // Clear the devices if no callback is provided to avoid calling the callback
        Logger::error("Wheel Driver: Init error: no callback function provided for input manager");
        return -2; // Nothing to configure
    }

    //initialize the GPIO pin for each device
    for(auto& device : CESP_WheelDriver::_devices) {
        device->encoder = new EncoderStepCounter(device->s1_gpio, device->s2_gpio);
        if(device->encoder == nullptr) {
            Logger::error("Wheel Driver: Init error: unable to create encoder object for device %d", device->deviceId);
            continue; // Skip this device
        }

        //create interrupts from the GPIO pins
        device->interrupt_s1 = digitalPinToInterrupt(device->s1_gpio);
        device->interrupt_s2 = digitalPinToInterrupt(device->s2_gpio);
        if(device->interrupt_s1 == NOT_AN_INTERRUPT || device->interrupt_s2 == NOT_AN_INTERRUPT) {
            Logger::error("Wheel Driver: Init error: unable to create interrupt for device %d", device->deviceId);
            delete device->encoder;
            device->encoder = nullptr; // Clean up the encoder object
            continue; // Skip this device
        }

        //update the encoder state the first time
        device->encoder->begin();
        device->encoder->tick();
        device->last_change_time_ms = millis();

        //attach interrupts for the encoder pins
        uint8_t deviceId = device->deviceId;
        attachInterrupt(device->interrupt_s1, CESP_WheelDriver::update_device_isr, CHANGE);
        attachInterrupt(device->interrupt_s2, CESP_WheelDriver::update_device_isr, CHANGE);
    }
    return 0;
}

void CESP_WheelDriver::update_device_state(const std::unique_ptr<InternalDeviceInfo>& device){

    if(device->encoder == nullptr) {
        return; // Skip this device if encoder is not initialized
    }

    //update encoder state
    int delta = device->encoder->getPosition();
    if(delta != 0) {
        uint32_t now = millis();
        if(now - device->last_change_time_ms > device->debounce_time_ms) {
            
            delta = device->invert ? -delta : delta; // Invert the delta if needed

            //update state and time
            device->last_change_time_ms = now;
            device->encoder->reset();
            _input_manager_callback(device->deviceId, delta);
        }
        
    }
}

/// Deinitialize the encoders and detach interrupts
int CESP_WheelDriver::deinit(void* arg){

    for(auto& device : CESP_WheelDriver::_devices) {
        if(device->encoder != nullptr) {
            detachInterrupt(device->interrupt_s1);
            detachInterrupt(device->interrupt_s2);
        }
    }
    delay(10); // waits any interrupt call to finish
    for(auto& device : CESP_WheelDriver::_devices) {
        if(device->encoder != nullptr) {
            delete device->encoder; // Clean up the encoder object
            device->encoder = nullptr;
        }
    }
    return 0; // Success
}

//update device state
int CESP_WheelDriver::update(void* args){
    for(auto& device : CESP_WheelDriver::_devices) {
        update_device_state(device);
    }
    return 0;

}