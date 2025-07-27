// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/chibi_kernel.h"
#include "core/base_devices/button.h"
#include "core/logging/logging.h"
#include "core/base_devices/wheel.h"
#include "core/kernel/components/interface_manager.h"
#include "core/kernel/device/display_device.h"
#include "core/kernel/components/device_manager.h"

ChibiKernel* ChibiKernel::instance = nullptr;

ChibiKernel::ChibiKernel() : 
  _task_manager(this)
{
  _interfaceManager = new InterfaceManager();
  _deviceManager = new DeviceManager();
}

int ChibiKernel::init(){

  instance = this; // Set the static instance pointer

  Logger::init();
  Logger::info("Starting ChibiKernel..");
  //get what core is reserved for kernel and for usermode
  _kernelCoreId = xPortGetCoreID();
  _userModeCoreId = 1 - _kernelCoreId;
  _slow_loop_timer = millis();

  _task_manager.Init(); // Initialize the task manager

  Logger::info("Kernel running on core %d", _kernelCoreId);

  return 0;
}

void ChibiKernel::init_kernel_devices(){

  _deviceManager->init_hid_devices(ChibiKernel::input_interrupt_callback); // Initialize control input devices
  _deviceManager->init_display_devices(); // Initialize display devices
}

// Static wrapper function for wheel inputs
void ChibiKernel::input_interrupt_callback(InputEvent &event){
  // Forward the call to the instance's _input_manager
  if (instance) {
    instance->get_input_manager().input_interrupt_callback(event); // Call the input manager's callback function
  }
}

int ChibiKernel::register_hid_device(HIDDevice* device){
  return _deviceManager->register_hid_device(device); // Register the device with the device manager
}

int ChibiKernel::register_display_device(DisplayDevice* device){
  return _deviceManager->register_display_device(device); // Register the device with the device manager
}

DisplayDevice* ChibiKernel::getDisplayDevice(uint32_t deviceId){
  return _deviceManager->get_display_device_by_id(deviceId); // Get the display device by id
}

void ChibiKernel::update_device_state(){
  _deviceManager->update_hid_devices_state(); // Update the state of control input devices
}

void ChibiKernel::loop(){
  // update hardware state
  update_device_state();

  //unfrequent tasks
  if(millis() - _slow_loop_timer){
    _slow_loop_timer = millis();
    _task_manager.update();
    _input_manager.update();
  }

}

bool ChibiKernel::registerI2cInterface(int bus, int sda_pin, int scl_pin){
  return _interfaceManager->registerI2cInterface(bus, sda_pin, scl_pin);
}

TwoWire* ChibiKernel::getI2cInterface(int bus){
  return _interfaceManager->getI2cInterface(bus);
}

int ChibiKernel::register_input_listener(InputListener *&listener){
  return _input_manager.createInputListener(listener); // Register a new input listener
}

InputEvent ChibiKernel::getNavUpEvent() const{
  return _upNavEvent; // Get the navigation up event
}

InputEvent ChibiKernel::getNavDownEvent() const{
  return _downNavEvent; // Get the navigation down event
}

InputEvent ChibiKernel::getNavSelectEvent() const{
  return _selectNavEvent; // Get the navigation select event
}

void ChibiKernel::setNavUpEvent(InputEvent event){
  _upNavEvent = event;
}

void ChibiKernel::setNavDownEvent(InputEvent event){
  _downNavEvent = event;
}

void ChibiKernel::setNavSelectEvent(InputEvent event){
  _selectNavEvent = event; // Set the navigation select event
}

/**
 * * @brief Creates a new program and registers it with the program manager.
 * * @param program The program to be created and registered.
 * * @return The task ID of the created program, or an error code if the program could not be created.
 */
int ChibiKernel::createProgram(CESP_Program program){
  return _program_manager.register_program(program); // Register the program with the program manager
}

/**
 * * @brief Starts a program by its name.
 * * @param programName The name of the program to start.
 * * @return The task ID of the started program, or an error code if the program could not be started.
 */
int ChibiKernel::startProgram(std::string programName){
  CESP_Program* program = nullptr; // Pointer to hold the program
  if(!_program_manager.get_program_by_name(programName, program)){
    return -1; // Error: program not found
  }

  // Create a new task for the program
  int taskID = _task_manager.create_new_task(program);
  if(taskID < 0){
    return -2; // Error: unable to create task
  }
  int started = _task_manager.start_task(taskID); // Start the task

  if(started < 0){
    return -3; // Error: unable to start task
  }
  return taskID; // Return the task ID
}

/**
 * * @brief Forcefully kills a task by its ID.
 * * @param taskID The ID of the task to kill.
 * * @return 0 on success, or an error code if the task could not
 */
int ChibiKernel::killTask(const uint8_t taskID){

  return _task_manager.kill_task(taskID); // Kill the program by its ID
}

/**
 * * @brief Gracefully quit a task by its ID.
 * * @param taskID The ID of the task to quit.
 */
int ChibiKernel::quitTask(const uint8_t taskID){
  return _task_manager.quit_task(taskID); // Gracefully quit the program by its ID
}

/**
 * * @brief checks whether a task is alive or not
 * * @param taskID The ID of the task to check
 */
bool ChibiKernel::isTaskRunning(const uint8_t taskID){
  return _task_manager.is_task_alive(taskID);
}

/**
 * * @brief checks whether a program is alive or not
 * * @param programName The name of the program to check
 */
bool ChibiKernel::isProgramRunning(const std::string programName){
  return _task_manager.is_program_alive(programName);
}
