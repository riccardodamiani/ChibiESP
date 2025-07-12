#include "core/kernel/device/control_input_device.h"
#include "core/logging/logging.h"

ControlInputDevice::ControlInputDevice(uint32_t deviceId){
    _deviceId = deviceId;
}

/**
 * @brief Initialize the control input device
 * @details Initialize the hardware such as set GPIO pins, attach interrupts for each device, ecc..
 */
int ControlInputDevice::init(ControlDeviceInitStruct_t& init_struct){

}

/**
 * @brief Deinitialize the control input device
 * @details Deinitialize the hardware such as detach interrupts, ecc..
 */
int ControlInputDevice::deinit(){

}

/**
 * @brief Update the control input device
 * @details Update the state of the devices if the device is not using interrupts for hardware state changes
 */
int ControlInputDevice::update(){

}

/**
 * @brief Get device information
 */
int ControlInputDevice::get_device_info(void* arg){

}
