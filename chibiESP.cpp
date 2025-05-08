#include "chibiESP.h"
#include "core/driver/button.h"
#include "core/logging/logging.h"
#include "core/driver/wheel.h"
#include "core/interface/interface_manager.h"
#include "core/driver/display_driver.h"
#include "core/kernel/driver_manager.h"

//include cpp files that needs to be compiled
#include "core/logging/logging.cpp"
#include "core/input/input_manager.cpp"
#include "core/input/input_listener.cpp"
#include "core/input/user_input_interface.cpp"
#include "core/driver/control_input_driver.cpp"
#include "core/driver/button.cpp"
#include "core/driver/display_driver.cpp"
#include "core/driver/ssd1306.cpp"
#include "core/driver/wheel.cpp"
#include "core/program/task.cpp"
#include "core/program/task_manager.cpp"
#include "core/program/program_manager.cpp"
#include "core/graphics/gui_element.cpp"
#include "core/graphics/task_view_renderer.cpp"
#include "core/graphics/view.cpp"
#include "core/interface/interface_manager.cpp"
#include "core/program/task_interface.cpp"
#include "core/kernel/driver_manager.cpp"


ChibiESP* ChibiESP::instance = nullptr;

ChibiESP::ChibiESP() : 
  _task_manager(this)
{
  _interfaceManager = new InterfaceManager();
  _driverManager = new DriverManager();
}

int ChibiESP::init(){

  instance = this; // Set the static instance pointer

  Logger::init();
  Logger::info("Starting ChibiESP..");
  //get what core is reserved for kernel and for usermode
  _kernelCoreId = xPortGetCoreID();
  _userModeCoreId = 1 - _kernelCoreId;
  _slow_loop_timer = millis();

  Logger::info("Kernel running on core: %d", _kernelCoreId);

  return 0;
}

void ChibiESP::init_kernel_drivers(){

  _driverManager->init_control_input_drivers(ChibiESP::input_interrupt_callback); // Initialize control input drivers
  _driverManager->init_display_drivers(this); // Initialize display drivers
}

// Static wrapper function for wheel inputs
void ChibiESP::input_interrupt_callback(InputEvent &event){
  // Forward the call to the instance's _input_manager
  if (instance) {
    instance->get_input_manager().input_interrupt_callback(event); // Call the input manager's callback function
  }
}

int ChibiESP::register_control_input_driver_module(ControlInputDriver* driver){
  return _driverManager->register_control_input_driver_module(driver); // Register the driver with the driver manager
}

int ChibiESP::register_display_driver_module(DisplayDriver* driver){
  return _driverManager->register_display_driver_module(driver); // Register the driver with the driver manager
}

DisplayDriver* ChibiESP::getDisplayDriver(std::string name){
  return _driverManager->get_display_driver_by_name(name); // Get the display driver by name
}

void ChibiESP::update_driver_state(){
  _driverManager->update_control_input_drivers_state(); // Update the state of control input drivers
}

void ChibiESP::loop(){
  // update hardware state
  update_driver_state();

  //unfrequent tasks
  if(millis() - _slow_loop_timer){
    _slow_loop_timer = millis();
    _task_manager.update();
    _input_manager.update();
  }

}

bool ChibiESP::registerI2cInterface(int bus, int sda_pin, int scl_pin){
  return _interfaceManager->registerI2cInterface(bus, sda_pin, scl_pin);
}

TwoWire* ChibiESP::getI2cInterface(int bus){
  return _interfaceManager->getI2cInterface(bus);
}

int ChibiESP::register_input_listener(InputListener *&listener){
  return _input_manager.createInputListener(listener); // Register a new input listener
}

InputEvent ChibiESP::getNavUpEvent(){
  InputEvent event;
  event.type = InputEventType::INPUT_EVENT_WHEEL;
  event.deviceID = 0;
  event.eventData = 1;
  event.deviceEventType = static_cast<uint8_t>(WheelEventType::WHEEL_EVENT_MOVED);
  return event;
}

InputEvent ChibiESP::getNavDownEvent(){
  InputEvent event;
  event.type = InputEventType::INPUT_EVENT_WHEEL;
  event.deviceID = 0;
  event.eventData = -1;
  event.deviceEventType = static_cast<uint8_t>(WheelEventType::WHEEL_EVENT_MOVED);
  return event;
}

InputEvent ChibiESP::getNavSelectEvent(){
  InputEvent event;
  event.type = InputEventType::INPUT_EVENT_KEY;
  event.deviceID = 2;
  event.eventData = 0;
  event.deviceEventType = static_cast<uint8_t>(KeyEventType::KEY_EVENT_RELEASED);
  return event;
}

/**
 * * @brief Creates a new program and registers it with the program manager.
 * * @param program The program to be created and registered.
 * * @return The task ID of the created program, or an error code if the program could not be created.
 */
int ChibiESP::createProgram(CESP_Program program){
  return _program_manager.register_program(program); // Register the program with the program manager
}

/**
 * * @brief Starts a program by its name.
 * * @param programName The name of the program to start.
 * * @return The task ID of the started program, or an error code if the program could not be started.
 */
int ChibiESP::startProgram(std::string programName){
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
int ChibiESP::killTask(const uint8_t taskID){

  return _task_manager.kill_task(taskID); // Kill the program by its ID
}

/**
 * * @brief Gracefully quit a task by its ID.
 * * @param taskID The ID of the task to quit.
 */
int ChibiESP::quitTask(const uint8_t taskID){
  return _task_manager.quit_task(taskID); // Gracefully quit the program by its ID
}

/**
 * * @brief checks whether a task is alive or not
 * * @param taskID The ID of the task to check
 */
bool ChibiESP::isTaskRunning(const uint8_t taskID){
  return _task_manager.is_task_alive(taskID);
}

/**
 * * @brief checks whether a program is alive or not
 * * @param programName The name of the program to check
 */
bool ChibiESP::isProgramRunning(const std::string programName){
  return _task_manager.is_program_alive(programName);
}
