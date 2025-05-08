#include "core/kernel/components/driver_manager.h"
#include "core/kernel/driver/control_input_driver.h"
#include "core/kernel/driver/display_driver.h"
#include "core/logging/logging.h"
#
#include <mutex>

class ChibiESP;

int DriverManager::register_control_input_driver_module(ControlInputDriver* driver){
    std::lock_guard<std::mutex> lock(_controlInputDriverMutex); // Lock the mutex for thread safety
    for (const auto& existing_driver : _regControlInputDrivers) {
        if (existing_driver.driver->get_name() == driver->get_name()) {
            Logger::error("Driver %s is already registered", driver->get_name().c_str());
            return -1; // Driver already registered
        }
    }

    // Register the new driver
    ControlInputControlStruct_t driver_struct;
    driver_struct.driver = driver;
    _regControlInputDrivers.push_back(driver_struct);
    Logger::info("Driver %s registered", driver->get_name().c_str());
    return 0; // Success
}

int DriverManager::register_display_driver_module(DisplayDriver* driver){
    std::lock_guard<std::mutex> lock(_displayDriverMutex); // Lock the mutex for thread safety
    for (const auto& existing_driver : _regDisplayDrivers) {
        if (existing_driver.driver->get_name() == driver->get_name()) {
            Logger::error("Display driver %s is already registered", driver->get_name().c_str());
            return -1; // Driver already registered
        }
    }

    // Register the new driver
    DisplayDriverControlStruct_t driver_struct;
    driver_struct.driver = driver;
    _regDisplayDrivers.push_back(driver_struct);
    Logger::info("Display driver %s registered", driver->get_name().c_str());
    return 0; // Success
}

void DriverManager::init_control_input_drivers(void input_interrupt_callback(InputEvent &)){
    // Initialize control input drivers
    for (auto& inputDriver : _regControlInputDrivers) {
        ControlDriverInitStruct_t init_struct;
        init_struct.input_interrupt = input_interrupt_callback; // Set the input interrupt callback function
        if(inputDriver.driver->init(init_struct) < 0){
            Logger::error("Failed to initialize control input driver %s", inputDriver.driver->get_name().c_str());
        }
    }
}

void DriverManager::init_display_drivers(ChibiESP* kernelInstance){
    // Initialize display drivers
    for (auto& displayDriver : _regDisplayDrivers) {
        DisplayDriverInitStruct init_struct;
        init_struct.kernelInstance = kernelInstance; // Set the kernel instance
        if(displayDriver.driver->init(&init_struct) < 0){
            Logger::error("Failed to initialize display driver %s", displayDriver.driver->get_name().c_str());
        }
    }
}

int DriverManager::update_control_input_drivers_state(){
    std::lock_guard<std::mutex> lock(_controlInputDriverMutex); // Lock the mutex for thread safety
    for (auto& inputDriver : _regControlInputDrivers) {
        if(inputDriver.driver->update() < 0){
            //Logger::error("Failed to update control input driver %s", inputDriver.driver->get_name().c_str());
        }
    }
    return 0; // Success
}

DisplayDriver* DriverManager::get_display_driver_by_name(std::string name){
    std::lock_guard<std::mutex> lock(_displayDriverMutex); // Lock the mutex for thread safety
    for (auto& displayDriver : _regDisplayDrivers) {
        if(displayDriver.driver->get_name() == name){
            return displayDriver.driver; // Return the driver if found
        }
    }
    return nullptr; // Driver not found
}