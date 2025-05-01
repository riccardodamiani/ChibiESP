#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "core/input/input_manager.h" //for InputEvent

#include <deque>
#include <mutex>

class InputListener {
public:
    bool pushEvent(InputEvent event);
    bool popEvent(InputEvent &event);
    void clearEvents();
private:
    std::deque<InputEvent> _events; // Vector to store input events
    std::mutex _eventMutex; // Mutex for thread safety when accessing events
};

#endif //INPUT_LISTENER_H