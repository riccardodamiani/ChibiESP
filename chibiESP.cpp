#include "chibiESP.h"
#include "core/driver/button.h"
#include "core/logging/logging.h"
#include "core/driver/wheel.h"

//include cpp files that needs to be compiled
#include "core/logging/logging.cpp"
#include "core/input/input_manager.cpp"
#include "core/input/input_listener.cpp"
#include "core/input/user_input_interface.cpp"
#include "core/driver/driver.cpp"
#include "core/driver/button.cpp"
#include "core/driver/wheel.cpp"
#include "core/program/task.cpp"
#include "core/program/task_manager.cpp"
#include "core/program/program_manager.cpp"


ChibiESP* ChibiESP::instance = nullptr;

ChibiESP::ChibiESP() : 
  _task_manager(this)
{

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

  //unfrequent tasks
  if(millis() - _slow_loop_timer){
    _slow_loop_timer = millis();
    _task_manager.update();
    _input_manager.update();
  }

}

int ChibiESP::register_input_listener(InputListener *&listener){
  return _input_manager.createInputListener(listener); // Register a new input listener
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
