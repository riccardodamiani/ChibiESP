/**
 * @file input_manager.h
 * @brief Input Manager header file
 * @details The InputManager receive inputs events from the control input drivers (Button, wheel, etc) and dispatch them to the registered input destinations (applications, window input manager, etc).
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <vector>
#include <mutex>

enum class InputEventType {
    INPUT_EVENT_KEY = 0,
    INPUT_EVENT_WHEEL = 1
};

enum class KEY_EVENT_TYPE{
    KEY_EVENT_PRESSED = 0,
    KEY_EVENT_RELEASED = 1
};

enum class WHEEL_EVENT_TYPE{
    WHEEL_EVENT_MOVED = 0
};


struct InputEvent{
    InputEventType type;
    uint8_t deviceID;
    uint8_t deviceEventType;
    int32_t eventData; //contains information such as how much the wheel has moved
};

struct InputDestination{
    uint8_t id; // ID of the input destination
    void (*event_callback)(InputEvent); // Callback function for input events
};

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    int registerInputDestination(void (*event_callback)(InputEvent));
    int unregisterInputDestination(uint8_t id);

    //internal use only
    void button_callback(uint8_t buttonID, bool state);
    void wheel_callback(uint8_t deviceID, int delta);
private:
    void dispatchEvent(InputEvent event);

    std::vector <InputDestination> _inputDestinations; // List of input destination callbacks
    std::mutex _destMutex; // Mutex for thread safety
};

#endif //INPUT_MANAGER_H