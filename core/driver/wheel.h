#ifndef WHEEL_H
#define WHEEL_H

#include <EncoderStepCounter.h>
#include <vector>
#include <atomic>
#include <memory>

#include "core/driver/control_input_driver.h"
#include "core/input/input_structs.h"

struct CESP_WheelConfigStruct{
  uint8_t deviceID;
  uint8_t s1_gpio, s2_gpio; // GPIO pins for the wheel
  bool invert; // invert rotation direction
  uint32_t debounce_time_ms; // Debounce time in milliseconds
};

struct CESP_WheelDriverConfigStruct{
  std::vector <CESP_WheelConfigStruct> devices;
};

class CESP_WheelDriver : public ControlInputDriver {
public:
  int configure(void* arg) override;
  int init(ControlDriverInitStruct_t& init_struct) override;
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

  void update_device_state(const std::unique_ptr<InternalDeviceInfo>& device);
  static void update_device_isr();

  static std::vector<std::unique_ptr<InternalDeviceInfo>> _devices;
  void (*_input_interrupt)(InputEvent&); // Callback function for button events
};

#endif //WHEEL_H