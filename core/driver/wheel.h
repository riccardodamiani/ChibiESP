#ifndef WHEEL_H
#define WHEEL_H

#include <EncoderStepCounter.h>
#include <vector>
#include <atomic>
#include <memory>

#include "core/driver/driver.h"

struct CESP_WheelConfigStruct{
  uint8_t deviceID;
  uint8_t s1_gpio, s2_gpio; // GPIO pins for the wheel
  bool invert; // invert rotation direction
  uint32_t debounce_time_ms; // Debounce time in milliseconds
};

struct CESP_WheelDriverConfigStruct{
  std::vector <CESP_WheelConfigStruct> devices;
};

struct CESP_WheelDriverInitStruct{
  void (*callback)(uint8_t, int); // Callback function for wheel events
};

class CESP_WheelDriver : public CESP_Driver {
public:
  int configure(void* arg) override;
  int init(void* arg) override;
  int deinit(void* arg) override;
  int update(void* args) override;

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
  void (*_input_manager_callback)(uint8_t, int); // Callback function for button events
};

#endif //WHEEL_H