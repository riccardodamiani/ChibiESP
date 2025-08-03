// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include <chibiESP.h>
#include <core/kernel/device/device.h>
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

  chibiESP.register_device(button1);
  chibiESP.register_device(button2);
  chibiESP.register_device(button3);

  //initialize the wheel encoder
  WheelDevice *wheel = new WheelDevice(3);
  WheelDeviceConfigStruct wheelConfig = {4, 6, true, 50};
  wheel->configure(wheelConfig);
  chibiESP.register_device(wheel);

  //initialize the I2C interface for the display
  chibiESP.registerI2cInterface(0, 9, 8);

  //initialize the display device
  SSD1306 *display = new SSD1306(4);
  SSD1306ConfigStruct displayConfig = {128, 64, 0};
  display->configure(displayConfig);
  chibiESP.register_device(display);

  //initialize all system devices
  chibiESP.init_kernel_devices();

  //set the default navigation up input event
  InputEvent upEvent;
  upEvent.type = InputEventType::INPUT_EVENT_WHEEL;
  upEvent.deviceID = wheel->getDeviceId();
  upEvent.eventData = 1; // 1 step 
  upEvent.deviceEventType = static_cast<uint8_t>(WheelEventType::WHEEL_EVENT_MOVED);
  chibiESP.setNavUpEvent(upEvent);

  //set the default navigation down input event
  InputEvent downEvent;
  downEvent.type = InputEventType::INPUT_EVENT_WHEEL;
  downEvent.deviceID = wheel->getDeviceId();
  downEvent.eventData = -1; //1 step in the opposite direction
  downEvent.deviceEventType = static_cast<uint8_t>(WheelEventType::WHEEL_EVENT_MOVED);
  chibiESP.setNavDownEvent(downEvent);

  //set the default navigation select input event
  InputEvent selectEvent;
  selectEvent.type = InputEventType::INPUT_EVENT_KEY;
  selectEvent.deviceID = 2;
  selectEvent.eventData = 0;
  selectEvent.deviceEventType = static_cast<uint8_t>(KeyEventType::KEY_EVENT_RELEASED);
  chibiESP.setNavSelectEvent(selectEvent);

  //enter setup loop
  user_setup_function();
}

void loop() {
  chibiESP.loop();

  delay(10);
}