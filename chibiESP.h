// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef CHIBI_ESP_H
#define CHIBI_ESP_H

#include "core/kernel/components/input_manager.h"
#include "core/structs/program.h"
#include "core/structs/input_structs.h"

#include <string>
#include <stdint.h>

class InputListener;
class DisplayDevice;
class InterfaceManager;
class TwoWire;
class ChibiKernel;
class HIDDevice;

class ChibiESP{
public:
  ChibiESP();
  ~ChibiESP();
  int init();
  void init_kernel_devices();
  void loop();
  int register_hid_device(HIDDevice* device);
  int register_display_device(DisplayDevice* device);

  //program functions
  int createProgram(CESP_Program program);
  int startProgram(std::string programName); // Start a program by name
  int killTask(const uint8_t taskID); // Kill a task by ID
  int quitTask(const uint8_t taskID); // Gracefully quit a task by ID
  bool isTaskRunning(const uint8_t taskID);
  bool isProgramRunning(const std::string programName);

  //input navigation events
  InputEvent getNavUpEvent() const;
  InputEvent getNavDownEvent() const;
  InputEvent getNavSelectEvent() const;
  void setNavUpEvent(InputEvent event);
  void setNavDownEvent(InputEvent event);
  void setNavSelectEvent(InputEvent event);

  //devices getter
  DisplayDevice* getDisplayDevice(uint32_t deviceId);

  //interfaces
  bool registerI2cInterface(int bus, int sda_pin, int scl_pin);
  TwoWire* getI2cInterface(int bus);

  //getters for cores reservations
  int getKernelCoreId() const;
  int getUserCoreId() const;
private:

  ChibiKernel *_kernel; // Pointer to the kernel instance
  
};

extern ChibiESP chibiESP; // Global instance of ChibiESP

#endif //CHIBI_ESP_H