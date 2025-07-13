// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "chibiESP.h"
#include "core/kernel/chibi_kernel.h"
#include "core/kernel/components/input_manager.h"
#include "core/structs/program.h"
#include "core/structs/input_structs.h"

//include cpp files that needs to be compiled
#include "core/kernel/chibi_kernel.cpp"
#include "core/logging/logging.cpp"
#include "core/kernel/components/input_manager.cpp"
#include "core/kernel/components/input_listener.cpp"
#include "core/kernel/device/control_input_device.cpp"
#include "core/kernel/device/display_device.cpp"
#include "core/kernel/components/task_manager.cpp"
#include "core/kernel/components/program_manager.cpp"
#include "core/kernel/components/device_manager.cpp"
#include "core/kernel/components/interface_manager.cpp"
#include "core/task/task.cpp"
#include "core/task/task_interface.cpp"
#include "core/task/gui/gui_element.cpp"
#include "core/task/gui/task_view_renderer.cpp"
#include "core/task/gui/view.cpp"
#include "core/base_devices/ssd1306.cpp"
#include "core/base_devices/wheel.cpp"
#include "core/base_devices/button.cpp"

ChibiESP chibiESP = ChibiESP(); // Global instance of ChibiESP

ChibiESP::ChibiESP()
{
  _kernel = new ChibiKernel(); // Initialize the kernel instance
}

 ChibiESP::~ChibiESP(){
  if(_kernel){
    delete _kernel; // Delete the kernel instance
    _kernel = nullptr;
  }
 }

 /**
  * * @brief Initializes the ChibiESP library.
  * * @details This function initializes the kernel and sets up the necessary components for the library to function. Must be called in the .ino file before using any other functions.
  */
int ChibiESP::init(){
  _kernel->init(); // Initialize the kernel
  return 0;
}

/**
 * * @brief Initializes the kernel devices.
 * * @details This function initializes the kernel devices, including input and display devices. Must be called in the .ino file after init().
 */
void ChibiESP::init_kernel_devices(){
  _kernel->init_kernel_devices(); // Initialize the kernel devices
}

/**
 * * @brief Registers a control input device.
 * * @param device The control input device to be registered.
 * * @return 0 on success, or an error code if the device could not be registered.
 * * @details This function registers a control input device with the kernel. The device must implement the ControlInputDevice interface.
 */
int ChibiESP::register_control_input_device(ControlInputDevice* device){
  return _kernel->register_control_input_device(device); // Register the device with the device manager
}

/**
 * * @brief Registers a display device
 * * @param device The display device to be registered.
 * * @return 0 on success, or an error code if the device could not be registered.
 * * @details This function registers a display device with the kernel. The device must implement the DisplayDevice interface.
 */
int ChibiESP::register_display_device(DisplayDevice* device){
  return _kernel->register_display_device(device); // Register the device with the device manager
}

/**
 * * @brief Gets the display device by id.
 * * @param deviceId the id of the display device to be retrieved.
 * * @return A pointer to the display device, or nullptr if not found.
 */
DisplayDevice* ChibiESP::getDisplayDevice(uint32_t deviceId){
  return _kernel->getDisplayDevice(deviceId); // Get the display device by id
}

/**
 * * @brief Main loop function for the ChibiESP library.
 * * @details This function should be called in the main loop of the Arduino sketch. It handles the kernel's loop function and updates the state of the system.
 */
void ChibiESP::loop(){
  _kernel->loop(); // Call the kernel's loop function

}

/**
 * * @brief Gets the kernel core ID.
 * * @return The core ID of the kernel.
 */
int ChibiESP::getKernelCoreId() const{
  return _kernel->getKernelCoreId(); // Get the kernel core ID
}

/**
 * * @brief Gets the user core ID.
 * * @return The core ID of the user mode.
 */
int ChibiESP::getUserCoreId() const{
  return _kernel->getUserCoreId(); // Get the user core ID
}

/**
 * * @brief Registers an I2C interface.
 * * @param bus The I2C bus number.
 * * @param sda_pin The SDA pin number.
 * * @param scl_pin The SCL pin number.
 * * @return true on success, false on failure.
 */
bool ChibiESP::registerI2cInterface(int bus, int sda_pin, int scl_pin){
  return _kernel->registerI2cInterface(bus, sda_pin, scl_pin);
}

/**
 * * @brief Gets the I2C interface by bus number.
 * * @param bus The I2C bus number.
 * * @return A pointer to the I2C interface, or nullptr if not found.
 */
TwoWire* ChibiESP::getI2cInterface(int bus){
  return _kernel->getI2cInterface(bus);
}

/**
 * * @brief Gets the navigation up event.
 * * @return The navigation up event.
 */
InputEvent ChibiESP::getNavUpEvent() const{
  return _kernel->getNavUpEvent(); // Get the navigation up event from the kernel
}

/**
 * * @brief Gets the navigation down event.
 * * @return The navigation down event.
 */
InputEvent ChibiESP::getNavDownEvent() const{
  return _kernel->getNavDownEvent(); // Get the navigation down event from the kernel
}

/**
 * * @brief Gets the navigation select event.
 * * @return The navigation select event.
 */
InputEvent ChibiESP::getNavSelectEvent() const{
  return _kernel->getNavSelectEvent(); // Get the navigation select event from the kernel
}

/**
 * * * @brief Sets the navigation up event.
 * * @param event The navigation up event to be set.
 */
void ChibiESP::setNavUpEvent(InputEvent event){
  _kernel->setNavUpEvent(event); // Set the navigation up event in the kernel
}

/**
 * * * @brief Sets the navigation down event.
 * * @param event The navigation down event to be set.
 */
void ChibiESP::setNavDownEvent(InputEvent event){
  _kernel->setNavDownEvent(event); // Set the navigation down event in the kernel
}

/**
 * * * @brief Sets the navigation select event.
 * * @param event The navigation select event to be set.
 */
void ChibiESP::setNavSelectEvent(InputEvent event){
  _kernel->setNavSelectEvent(event); // Set the navigation select event in the kernel
}

/**
 * * @brief Creates a new program and registers it with the program manager.
 * * @param program The program to be created and registered.
 * * @return The task ID of the created program, or an error code if the program could not be created.
 */
int ChibiESP::createProgram(CESP_Program program){
  return _kernel->createProgram(program); // Register the program with the program manager
}

/**
 * * @brief Starts a program by its name.
 * * @param programName The name of the program to start.
 * * @return The task ID of the started program, or an error code if the program could not be started.
 */
int ChibiESP::startProgram(std::string programName){
  return _kernel->startProgram(programName); // Start the program by its name
}

/**
 * * @brief Forcefully kills a task by its ID.
 * * @param taskID The ID of the task to kill.
 * * @return 0 on success, or an error code if the task could not
 */
int ChibiESP::killTask(const uint8_t taskID){
return _kernel->killTask(taskID); // Forcefully kill the program by its ID
}

/**
 * * @brief Gracefully quit a task by its ID.
 * * @param taskID The ID of the task to quit.
 */
int ChibiESP::quitTask(const uint8_t taskID){
  return _kernel->quitTask(taskID); // Gracefully quit the program by its ID
}

/**
 * * @brief checks whether a task is alive or not
 * * @param taskID The ID of the task to check
 */
bool ChibiESP::isTaskRunning(const uint8_t taskID){
  return _kernel->isTaskRunning(taskID); // Check if the task is alive
}

/**
 * * @brief checks whether a program is alive or not
 * * @param programName The name of the program to check
 */
bool ChibiESP::isProgramRunning(const std::string programName){
  return _kernel->isProgramRunning(programName); // Check if the program is alive
}
