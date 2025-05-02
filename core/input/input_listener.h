#ifndef INPUT_LISTENER_H
#define INPUT_LISTENER_H

#include "core/input/input_structs.h" //for InputEvent

#include <deque>
#include <mutex>
#include <atomic>

class InputListener {
public:
    InputListener(); // Default constructor
    ~InputListener() = default; // Default destructor
    bool getEvent (InputEvent &event);
    void clearEvents();
    bool pushEvent(InputEvent event);
    void destroy();
    bool isAlive();
private:
    std::deque<InputEvent> _events; // Vector to store input events
    std::mutex _eventMutex; // Mutex for thread safety when accessing events
    std::atomic <bool> _alive;
};

#endif //INPUT_LISTENER_H