#include <Arduino.h>

#include "chibiESP.h"
#include "driver/button.h"
#include "core/logging.h"
#include "driver/wheel.h"

//include cpp files to make sure that are compiled
#include "core/logging.cpp"
#include "core/input_manager.cpp"
#include "driver/driver.cpp"
#include "driver/button.cpp"
#include "driver/wheel.cpp"

ChibiESP* ChibiESP::instance = nullptr;

int ChibiESP::init(){

  instance = this; // Set the static instance pointer

  Logger::init();
  Logger::info("Starting ChibiESP..");
  //get what core is reserved for kernel and for usermode
  _kernelCoreId = xPortGetCoreID();
  _userModeCoreId = 1 - _kernelCoreId;

  Logger::info("Kernel running on core: %d", _kernelCoreId);

  return 0;
}

void ChibiESP::init_drivers(){

  //initialize control input drivers
  for (auto& driver : _drivers) {
    if (driver->get_name() == "ButtonDriver") {
      CESP_ButtonDriverInitStruct init_struct;
      init_struct.callback = ChibiESP::button_callback_wrapper; // Use static wrapper function
      driver->init(&init_struct);
    } else if (driver->get_name() == "WheelDriver") {
      CESP_WheelDriverInitStruct init_struct;
      init_struct.callback = ChibiESP::wheel_callback_wrapper; // Use static wrapper function
      driver->init(&init_struct);
    } else {

    }
  }
}

int ChibiESP::register_driver_module(CESP_Driver* driver){
  // Check if the driver is already registered
  for (const auto& existing_driver : _drivers) {
    if (existing_driver->get_name() == driver->get_name()) {
      Logger::error("Driver %s is already registered", driver->get_name().c_str());
      return -1; // Driver already registered
    }
  }

  // Register the new driver
  _drivers.push_back(driver);
  Logger::info("Driver %s registered", driver->get_name().c_str());
  return 0; // Success
}



// Static wrapper function for button inputs
void ChibiESP::button_callback_wrapper(uint8_t buttonID, bool state) {
  // Forward the call to the instance's _input_manager
  if (instance) {
    instance->get_input_manager().button_callback(buttonID, state);
  }
}

// Static wrapper function for wheel inputs
void ChibiESP::wheel_callback_wrapper(uint8_t buttonID, int delta) {
  // Forward the call to the instance's _input_manager
  if (instance) {
    instance->get_input_manager().wheel_callback(buttonID, delta);
  }
}

void ChibiESP::update_driver_state(){
  // Update the state of each driver
  for (int i = 0; i < _drivers.size(); ++i) {
    CESP_Driver* driver = _drivers[i];
    driver->update(nullptr); // Pass nullptr as argument for now
  }
}

void ChibiESP::loop(){
  // update hardware state
  update_driver_state();
  
}
