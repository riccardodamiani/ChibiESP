// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include <chibiESP.h>
#include <core/kernel/device/device.h>
#include <core/base_devices/button.h>
#include <core/logging/logging.h>
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

  //initialize the buttons
  ButtonDeviceConfigStruct button1Config = {1, true, 50};
  ButtonDeviceConfigStruct button2Config = {2, true, 50};
  ButtonDeviceConfigStruct button3Config = {7, true, 50};

  ButtonDevice *button1 = new ButtonDevice(0);
  ButtonDevice *button2 = new ButtonDevice(1);
  ButtonDevice *button3 = new ButtonDevice(2);
  button1->configure(button1Config);
  button2->configure(button2Config);
  button3->configure(button3Config);

  chibiESP.register_control_input_device(button1);
  chibiESP.register_control_input_device(button2);
  chibiESP.register_control_input_device(button3);

  //initialize the system devices
  chibiESP.init_kernel_devices();

  //start the user setup function
  user_setup_function();
}

void loop() {
  chibiESP.loop();

  delay(10);
}