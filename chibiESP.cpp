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
#include "core/kernel/driver/control_input_driver.cpp"
#include "core/kernel/driver/display_driver.cpp"
#include "core/kernel/components/task_manager.cpp"
#include "core/kernel/components/program_manager.cpp"
#include "core/kernel/components/driver_manager.cpp"
#include "core/kernel/components/interface_manager.cpp"
#include "core/task/task.cpp"
#include "core/task/task_interface.cpp"
#include "core/task/gui/gui_element.cpp"
#include "core/task/gui/task_view_renderer.cpp"
#include "core/task/gui/view.cpp"
#include "core/example_drivers/ssd1306.cpp"
#include "core/example_drivers/wheel.cpp"
#include "core/example_drivers/button.cpp"

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
 * * @brief Initializes the kernel drivers.
 * * @details This function initializes the kernel drivers, including input and display drivers. Must be called in the .ino file after init().
 */
void ChibiESP::init_kernel_drivers(){
  _kernel->init_kernel_drivers(); // Initialize the kernel drivers
}

/**
 * * @brief Registers a control input driver module.
 * * @param driver The control input driver to be registered.
 * * @return 0 on success, or an error code if the driver could not be registered.
 * * @details This function registers a control input driver module with the kernel. The driver must implement the ControlInputDriver interface.
 */
int ChibiESP::register_control_input_driver_module(ControlInputDriver* driver){
  return _kernel->register_control_input_driver_module(driver); // Register the driver with the driver manager
}

/**
 * * @brief Registers a display driver module.
 * * @param driver The display driver to be registered.
 * * @return 0 on success, or an error code if the driver could not be registered.
 * * @details This function registers a display driver module with the kernel. The driver must implement the DisplayDriver interface.
 */
int ChibiESP::register_display_driver_module(DisplayDriver* driver){
  return _kernel->register_display_driver_module(driver); // Register the driver with the driver manager
}

/**
 * * @brief Gets the display driver by name.
 * * @param name The name of the display driver to be retrieved.
 * * @return A pointer to the display driver, or nullptr if not found.
 */
DisplayDriver* ChibiESP::getDisplayDriver(std::string name){
  return _kernel->getDisplayDriver(name); // Get the display driver by name
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
InputEvent ChibiESP::getNavUpEvent(){
  return _kernel->getNavUpEvent(); // Get the navigation up event from the kernel
}

/**
 * * @brief Gets the navigation down event.
 * * @return The navigation down event.
 */
InputEvent ChibiESP::getNavDownEvent(){
  return _kernel->getNavDownEvent(); // Get the navigation down event from the kernel
}

/**
 * * @brief Gets the navigation select event.
 * * @return The navigation select event.
 */
InputEvent ChibiESP::getNavSelectEvent(){
  return _kernel->getNavSelectEvent(); // Get the navigation select event from the kernel
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
