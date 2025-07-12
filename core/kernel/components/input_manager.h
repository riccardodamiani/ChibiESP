// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

/**
 * @file input_manager.h
 * @brief Input Manager header file
 * @details The InputManager receive inputs events from the control input devices (Button, wheel, etc) and dispatch them to the registered input destinations (applications, window input manager, etc).
 */

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>
#include <map>
#include <mutex>

#include "core/structs/input_structs.h"

class InputListener;

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    int createInputListener(InputListener *&listener);
    void update();

    //internal use only
    void input_interrupt_callback(InputEvent &event);
private:
    void dispatchEvent(InputEvent event);

    std::map <uint16_t, InputListener*> _inputListeners; // List of input destination callbacks
    std::mutex _mutex; // Mutex for thread safety
};

#endif //INPUT_MANAGER_H