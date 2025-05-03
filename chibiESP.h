#ifndef CHIBI_ESP_H
#define CHIBI_ESP_H

#include "core/input/input_manager.h"
#include "core/program/program_manager.h"
#include "core/program/task_manager.h"

class InputListener;
class CESP_Driver;

class ChibiESP{
public:
  ChibiESP();
  
  int init();
  void loop();
  void init_kernel_drivers();
  InputManager& get_input_manager() { return _input_manager; } // Getter for input manager instance
  int register_driver_module(CESP_Driver* driver);

  //program functions
  int createProgram(CESP_Program program);
  int startProgram(std::string programName); // Start a program by name
  int killTask(const uint8_t taskID); // Kill a task by ID
  int quitTask(const uint8_t taskID); // Gracefully quit a task by ID
  bool isTaskRunning(const uint8_t taskID);
  bool isProgramRunning(const std::string programName);

  //input functions (Internal use only)
  int register_input_listener(InputListener *&listener); // Register an input listener
private:
  static void button_callback_wrapper(uint8_t buttonID, bool state);
  static void wheel_callback_wrapper(uint8_t buttonID, int delta);
  void update_driver_state();

  static ChibiESP* instance;

  int _kernelCoreId, _userModeCoreId;
  InputManager _input_manager; // Input manager instance
  CESP_ProgramManager _program_manager; // Program manager instance
  CESP_TaskManager _task_manager; // Task manager instance
  std::vector <CESP_Driver*> _drivers; // List of drivers registered

  uint32_t _slow_loop_timer;
};

#endif //CHIBI_ESP_H