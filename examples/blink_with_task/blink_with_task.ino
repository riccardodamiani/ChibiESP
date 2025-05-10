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

#include "button_program.h"
#include "blink_program.h"

#include <vector>
#include <memory>

void user_setup_function(){
  CESP_Program buttonProgram("button program", button_program_setup, button_program_loop, button_program_closeup );
  CESP_Program blinkProgram("blink program", blink_program_setup, blink_program_loop, blink_program_closeup );

  chibiESP.createProgram(buttonProgram);
  chibiESP.createProgram(blinkProgram);

  chibiESP.startProgram("button program");
}

void setup() {
  chibiESP.init();

  CESP_ButtonDriver *BDriver = new CESP_ButtonDriver();
  CESP_ButtonDriverConfigStruct BConfig;
  CESP_ButtonConfigStruct button0 = {0, 1, true, 50};
  CESP_ButtonConfigStruct button1 = {1, 2, true, 50};
  CESP_ButtonConfigStruct button2 = {2, 7, true, 50};
  BConfig.devices.push_back(button0);
  BConfig.devices.push_back(button1);
  BConfig.devices.push_back(button2);
  BDriver->configure(&BConfig);

  chibiESP.register_control_input_driver_module(BDriver);

  chibiESP.init_kernel_drivers();

  user_setup_function();
}

void loop() {
  chibiESP.loop();

  delay(10);
}