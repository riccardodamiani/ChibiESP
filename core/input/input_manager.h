/**
 * @file input_manager.h
 * @brief Input Manager header file
 * @details The InputManager receive inputs events from the control input drivers (Button, wheel, etc) and dispatch them to the registered input destinations (applications, window input manager, etc).
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <map>
#include <mutex>

#include "core/input/input_structs.h"

class chibiESP;
class InputListener;

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    int createInputListener(InputListener *&listener);
    void update();

    //internal use only
    void button_callback(uint8_t buttonID, bool state);
    void wheel_callback(uint8_t deviceID, int delta);
private:
    void dispatchEvent(InputEvent event);

    std::map <uint16_t, InputListener*> _inputListeners; // List of input destination callbacks
    std::mutex _mutex; // Mutex for thread safety
};

#endif //INPUT_MANAGER_H