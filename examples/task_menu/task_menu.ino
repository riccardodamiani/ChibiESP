#include <chibiESP.h>
#include <core/base_devices/button.h>
#include <core/base_devices/wheel.h>
#include <core/base_devices/ssd1306.h>
#include <core/logging/logging.h>
#include <core/structs/program.h>

#include "menu_program.h"

#include <vector>
#include <memory>

void user_setup_function(){
  CESP_Program menuProgram("menu program", menu_program_setup, menu_program_loop, menu_program_closeup );

  chibiESP.createProgram(menuProgram);

  chibiESP.startProgram("menu program");
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

  //initialize the wheel encoder
  WheelDevice *wheel = new WheelDevice(0);
  WheelDeviceConfigStruct wheelConfig = {4, 6, true, 50};
  wheel->configure(wheelConfig);
  chibiESP.register_control_input_device(wheel);

  //initialize the I2C interface for the display
  chibiESP.registerI2cInterface(0, 9, 8);

  //initialize the display device
  SSD1306 *display = new SSD1306(0);
  SSD1306ConfigStruct displayConfig = {128, 64, 0};
  display->configure(displayConfig);
  chibiESP.register_display_device(display);

  //initialize all system devices
  chibiESP.init_kernel_devices();

  //enter setup loop
  user_setup_function();
}

void loop() {
  chibiESP.loop();

  delay(10);
}