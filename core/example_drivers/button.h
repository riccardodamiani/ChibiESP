#ifndef BUTTON_H
#define BUTTON_H

#include <vector>
#include <string>
#include <atomic>
#include <memory>

#include "core/kernel/driver/control_input_driver.h"

struct CESP_ButtonConfigStruct{
    uint8_t deviceID;
    uint8_t gpio_pin; // GPIO pin for button
    bool normally_open; // normally opened or closed?
    uint32_t debounce_time_ms; // Debounce time in milliseconds
};

struct CESP_ButtonDriverConfigStruct{
    std::vector <CESP_ButtonConfigStruct> devices;
};

class CESP_ButtonDriver : public ControlInputDriver{
public:
    int configure(void* arg) override;
    int init(ControlDriverInitStruct_t& init_struct) override;
    int deinit() override;
    int update() override;
    int get_device_info(void* arg) override;

private:
struct InternalDeviceInfo{
    uint8_t deviceID;
    uint8_t gpio_pin;
    uint8_t normally_open; // normally opened or closed?
    uint32_t debounce_time_ms; // Debounce time in milliseconds
    std::atomic <uint32_t> last_change_time_ms; // Last time the button state changed
    std::atomic <uint8_t> last_state; // Last state of the button (pressed or not pressed)
};

    void update_device_state(const std::unique_ptr<InternalDeviceInfo>& device);

    std::vector<std::unique_ptr<InternalDeviceInfo>> _devices;
    void (*_input_interrupt)(InputEvent&); // Callback function for button events
};

#endif //BUTTON_H