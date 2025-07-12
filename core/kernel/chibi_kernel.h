#ifndef CHIBI_KERNEL_H
#define CHIBI_KERNEL_H

#include "core/kernel/components/input_manager.h"
#include "core/kernel/components/program_manager.h"
#include "core/kernel/components/task_manager.h"
#include "core/kernel/components/device_manager.h"

class InputListener;
class DisplayDevice;
class InterfaceManager;
class TwoWire;
class ControlInputDevice;

class ChibiKernel{
public:
  ChibiKernel();
  
  int init();
  void loop();
  void init_kernel_devices();
  InputManager& get_input_manager() { return _input_manager; } // Getter for input manager instance
  int register_control_input_device(ControlInputDevice* device);
  int register_display_device(DisplayDevice* device);

  //program functions
  int createProgram(CESP_Program program);
  int startProgram(std::string programName); // Start a program by name
  int killTask(const uint8_t taskID); // Kill a task by ID
  int quitTask(const uint8_t taskID); // Gracefully quit a task by ID
  bool isTaskRunning(const uint8_t taskID);
  bool isProgramRunning(const std::string programName);

  //input functions (Internal use only)
  int register_input_listener(InputListener *&listener); // Register an input listener

  //input navigation events
  InputEvent getNavUpEvent();
  InputEvent getNavDownEvent();
  InputEvent getNavSelectEvent();

  //devices getter
  DisplayDevice* getDisplayDevice(uint32_t deviceId);

  //interfaces
  bool registerI2cInterface(int bus, int sda_pin, int scl_pin);
  TwoWire* getI2cInterface(int bus);

  //core ids
  int getKernelCoreId() const { return _kernelCoreId; } // Getter for kernel core ID
  int getUserCoreId() const { return _userModeCoreId; } // Getter for user core ID

  static ChibiKernel* instance;
private:
  static void input_interrupt_callback(InputEvent &event);
  void update_device_state();

  int _kernelCoreId, _userModeCoreId;
  InputManager _input_manager; // Input manager instance
  InterfaceManager *_interfaceManager;
  DeviceManager* _deviceManager; // Device manager instance
  CESP_ProgramManager _program_manager; // Program manager instance
  CESP_TaskManager _task_manager; // Task manager instance
  std::vector <ControlInputDevice*> _controlInputDevices; // List of input devices registered
  std::vector <DisplayDevice*> _displayDevices; // List of devices registered

  uint32_t _slow_loop_timer;
};


#endif // CHIBI_KERNEL_H