#include <chibiESP.h>
#include <core/kernel/driver/control_input_driver.h>
#include <core/example_drivers/button.h>
#include <core/example_drivers/wheel.h>
#include <core/example_drivers/ssd1306.h>
#include <core/task/user_task.h>
#include <core/task/task_memory.h>
#include <core/task/task_interface.h>
#include <core/structs/input_structs.h>
#include <core/logging/logging.h>
#include <core/task/gui/view.h>
#include <core/task/gui/gui_event.h>
#include <core/structs/program.h>

#include "menu_program.h"

#include <vector>
#include <memory>

ChibiESP kernel;

void user_setup_function(){
  CESP_Program menuProgram("menu program", menu_program_setup, menu_program_loop, menu_program_closeup );

  kernel.createProgram(menuProgram);

  kernel.startProgram("menu program");
}

void setup() {
  kernel.init();

  CESP_ButtonDriver *BDriver = new CESP_ButtonDriver();
  CESP_ButtonDriverConfigStruct BConfig;
  CESP_ButtonConfigStruct button0 = {0, 1, true, 50};
  CESP_ButtonConfigStruct button1 = {1, 2, true, 50};
  CESP_ButtonConfigStruct button2 = {2, 7, true, 50};
  BConfig.devices.push_back(button0);
  BConfig.devices.push_back(button1);
  BConfig.devices.push_back(button2);
  BDriver->configure(&BConfig);

  CESP_WheelDriver *WDriver = new CESP_WheelDriver();
  CESP_WheelDriverConfigStruct WConfig;
  CESP_WheelConfigStruct navigationWheel = {0, 4, 6, true, 50};
  WConfig.devices.push_back(navigationWheel);
  WDriver->configure(&WConfig);

  kernel.register_control_input_driver_module(BDriver);
  kernel.register_control_input_driver_module(WDriver);

  kernel.registerI2cInterface(0, 9, 8);

  SSD1306 *displayDriver = new SSD1306();
  displayDriver->configure(nullptr);
  kernel.register_display_driver_module(displayDriver);

  kernel.init_kernel_drivers();

  user_setup_function();
}

void loop() {
  kernel.loop();

  delay(10);
}