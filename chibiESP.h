#ifndef CHIBI_ESP_H
#define CHIBI_ESP_H

#include "core/input_manager.h"
#include "driver/driver.h"

class ChibiESP{
public:
  int init();
  void loop();
  void init_drivers();
  InputManager& get_input_manager() { return _input_manager; } // Getter for input manager instance
  int register_driver_module(CESP_Driver* driver);
private:
  static void button_callback_wrapper(uint8_t buttonID, bool state);
  static void wheel_callback_wrapper(uint8_t buttonID, int delta);
  void update_driver_state();

  static ChibiESP* instance;

  int _kernelCoreId, _userModeCoreId;
  InputManager _input_manager; // Input manager instance
  std::vector <CESP_Driver*> _drivers; // List of drivers registered
};

#endif //CHIBI_ESP_H