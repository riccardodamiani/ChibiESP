#include "core/input/input_listener.h"
#include "core/input/input_manager.h"
#include "core/logging.h"

#include <vector>
#include <mutex>
#include <atomic>

namespace{
    const int MAX_EVENTS = 20;
};

InputListener::InputListener() : _alive(true) { // Constructor initializes the alive status to true

}

bool InputListener::pushEvent(InputEvent event){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    if (_events.size() < MAX_EVENTS) {
        _events.push_back(event); // Add the event to the vector
        //Logger::info("Pushed event, total size: %d", _events.size());
        return true; // Event added successfully
    } else {
        return false; // Event queue is full, event not added
    }
}

bool InputListener::getEvent(InputEvent &event){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    if (!_events.empty()) {
        event = _events.front(); // Get the front event
        _events.pop_front(); // Remove it from the queue
        //Logger::info("Popped event, total size: %d", _events.size());
        return true; // Event popped successfully
    }
    return false; // No events to pop
}

void InputListener::clearEvents(){
    std::lock_guard<std::mutex> lock(_eventMutex); // Lock the mutex for thread safety
    _events.clear(); // Clear all events in the queue
}

void InputListener::destroy(){
    _alive.store(false); // Set the alive status to false
}

bool InputListener::isAlive(){
    return _alive.load(); // Return the alive status of the listener
}