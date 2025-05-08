#include <chibiESP.h>
#include <core/driver/button.h>
#include <core/driver/control_input_driver.h>
#include <core/driver/wheel.h>
#include <core/program/program.h>
#include <core/program/user_task.h>
#include <core/program/task_memory.h>
#include <core/program/task_interface.h>
#include <core/input/input_structs.h>
#include <core/logging/logging.h>
#include <core/driver/ssd1306.h>
#include <core/graphics/view.h>
#include <core/graphics/gui_event.h>

#include "button_program.h"
#include "blink_program.h"

#include <vector>
#include <memory>

ChibiESP kernel;

void user_setup_function(){
  CESP_Program buttonProgram("button program", button_program_setup, button_program_loop, button_program_closeup );
  CESP_Program blinkProgram("blink program", blink_program_setup, blink_program_loop, blink_program_closeup );

  kernel.createProgram(buttonProgram);
  kernel.createProgram(blinkProgram);

  kernel.startProgram("button program");
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

  kernel.register_control_input_driver_module(BDriver);

  kernel.init_kernel_drivers();

  user_setup_function();
}

void loop() {
  kernel.loop();

  delay(10);
}