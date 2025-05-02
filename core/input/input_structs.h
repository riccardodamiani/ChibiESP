#ifndef INPUT_STRUCTS_H
#define INPUT_STRUCTS_H

#include <stdint.h>

enum class InputEventType {
    INPUT_EVENT_KEY = 0,
    INPUT_EVENT_WHEEL = 1
};

enum class KeyEventType{
    KEY_EVENT_PRESSED = 0,
    KEY_EVENT_RELEASED = 1
};

enum class WheelEventType{
    WHEEL_EVENT_MOVED = 0
};


struct InputEvent{
    InputEventType type;
    uint8_t deviceID;
    uint8_t deviceEventType;
    int32_t eventData; //contains information such as how much the wheel has moved
};

#endif //INPUT_STRUCTS_H