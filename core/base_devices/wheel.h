// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef WHEEL_H
#define WHEEL_H

#include <EncoderStepCounter.h>
#include <vector>
#include <atomic>
#include <memory>

#include "core/kernel/device/control_input_device.h"
#include "core/structs/input_structs.h"

const uint8_t MAX_WHEEL_DEVICES = 5; // Maximum number of wheel devices

struct WheelDeviceConfigStruct{
  uint8_t s1_gpio, s2_gpio; // GPIO pins for the wheel
  bool invert; // invert rotation direction
  uint32_t debounce_time_ms; // Debounce time in milliseconds
};

class WheelDevice : public ControlInputDevice {
public:
  WheelDevice(uint32_t deviceId);
  int configure(WheelDeviceConfigStruct config);
  int init(ControlDeviceInitStruct_t& init_struct) override;
  int deinit() override;
  int update() override;
  int get_device_info(void* arg) override;

private:
struct InternalDeviceInfo{
  uint8_t deviceId;
  EncoderStepCounter *encoder; // Encoder object for the wheel
  uint8_t s1_gpio, s2_gpio; // GPIO pins for the wheel
  uint32_t interrupt_s1, interrupt_s2; // Interrupt numbers for the GPIO pins
  uint8_t invert; // normally opened or closed?
  uint32_t debounce_time_ms; // Debounce time in milliseconds
  std::atomic <uint32_t> last_change_time_ms; // Last time the button state changed
};

  void update_device_state();
  static void update_isr();
  void device_update_isr();

  // Callback function for button events
  void (*_input_interrupt)(InputEvent&); 

  // Static vector to hold all instances of WheelDevice
  static std::vector <WheelDevice *> _instances;
  
  //device data
  InternalDeviceInfo* _device;
};

#endif //WHEEL_H