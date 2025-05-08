#include "core/kernel/driver/control_input_driver.h"
#include "core/logging/logging.h"

ControlInputDriver::ControlInputDriver(){
    _name = "ControlInputDriver"; // Default name, can be overridden in derived classes
}

/**
 * @brief Configure the control input driver
 * @details Initialize driver variables such as GPIO pins, ecc..
 */
int ControlInputDriver::configure(void* arg){

}

/**
 * @brief Initialize the control input driver
 * @details Initialize the hardware such as set GPIO pins, attach interrupts for each device, ecc..
 */
int ControlInputDriver::init(ControlDriverInitStruct_t& init_struct){

}

/**
 * @brief Deinitialize the control input driver
 * @details Deinitialize the hardware such as detach interrupts, ecc..
 */
int ControlInputDriver::deinit(){

}

/**
 * @brief Update the control input driver
 * @details Update the state of the devices if the driver is not using interrupts for hardware state changes
 */
int ControlInputDriver::update(){

}

/**
 * @brief Get device information
 */
int ControlInputDriver::get_device_info(void* arg){

}
