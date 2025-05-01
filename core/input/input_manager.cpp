#include "core/input/input_manager.h"
#include "core/logging.h"

/**
 * @brief Registers a new input destination with a callback function.
 * * @param event_callback Pointer to the callback function that will handle input events.
 * * @return The ID of the newly registered input destination, or -1 if the callback is invalid.
 */
int InputManager::registerInputDestination(void (*event_callback)(InputEvent)){

    if (event_callback == nullptr) {
        return -1; // Invalid callback function
    }

    std::lock_guard<std::mutex> lock(_destMutex); // Lock the mutex for thread safety

    InputDestination newDestination;
    newDestination.id = _inputDestinations.size(); // Assign a unique ID based on the current size of the vector
    newDestination.event_callback = event_callback; // Set the callback function

    _inputDestinations.push_back(newDestination); // Add the new destination to the list

    return newDestination.id; // Return the ID of the newly registered destination
}

/**
 * @brief Unregisters an input destination by its ID.
 * * @param id The ID of the input destination to unregister.
 * * @return 0 on success, -1 if the destination was not found, or -2 if the ID is invalid.
 */
int InputManager::unregisterInputDestination(uint8_t id){
    std::lock_guard<std::mutex> lock(_destMutex); // Lock the mutex for thread safety

    if (id >= _inputDestinations.size()) {
        return -2; // Invalid ID error
    }

    for(size_t i = 0; i < _inputDestinations.size(); ++i) {
        if (_inputDestinations[i].id == id) {
            _inputDestinations.erase(_inputDestinations.begin() + i); // Remove the destination from the list
            return 0; // Success
        }
    }

    return -1; // not found
}

void InputManager::button_callback(uint8_t buttonID, bool state){
    InputEvent event;
    event.type = InputEventType::INPUT_EVENT_KEY; // Set the event type to key event
    event.deviceID = buttonID; // Set the device ID to the button ID
    event.deviceEventType = state ? static_cast<uint8_t>(KEY_EVENT_TYPE::KEY_EVENT_PRESSED) : static_cast<uint8_t>(KEY_EVENT_TYPE::KEY_EVENT_RELEASED); // Set the device event type based on the state
    event.eventData = 0; // No additional data for key events

    Logger::info("Button %d %s", buttonID, state ? "pressed" : "released"); // Log the button event
    dispatchEvent(event); // Dispatch the event to registered destinations
}

void InputManager::wheel_callback(uint8_t deviceID, int delta){
    InputEvent event;
    event.type = InputEventType::INPUT_EVENT_WHEEL; // Set the event type to wheel event  
    event.deviceID = deviceID; // Set the device ID to the wheel device ID
    event.deviceEventType = static_cast<uint8_t>(WHEEL_EVENT_TYPE::WHEEL_EVENT_MOVED); // Set the device event type to wheel moved
    event.eventData = delta; // Set the event data to the delta value

    Logger::info("Wheel %d moved %d", deviceID, delta); // Log the wheel event

    dispatchEvent(event); // Dispatch the event to registered destinations
}

void InputManager::dispatchEvent(InputEvent event){
    std::lock_guard<std::mutex> lock(_destMutex); // Lock the mutex for thread safety

    for(const auto& destination : _inputDestinations) {
        if (destination.event_callback != nullptr) {
            destination.event_callback(event); // Call the callback function for each registered destination
        }
    }
}