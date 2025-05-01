#include "core/input/input_listener.h"
#include "core/input/input_manager.h"

#include <vector>
#include <mutex>

namespace{
    const int MAX_EVENTS = 20;
};

bool InputListener::pushEvent(InputEvent event){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    if (_events.size() < MAX_EVENTS) {
        _events.push_back(event); // Add the event to the vector
        return true; // Event added successfully
    } else {
        return false; // Event queue is full, event not added
    }
}

bool InputListener::popEvent(InputEvent &event){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    if (!_events.empty()) {
        event = _events.front(); // Get the front event
        _events.pop_front(); // Remove it from the queue
        return true; // Event popped successfully
    } else {
        return false; // No events to pop
    }
}

void InputListener::clearEvents(){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    _events.clear(); // Clear all events in the queue
}