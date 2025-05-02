#include "core/input/user_input_interface.h"
#include "core/input/input_listener.h"
#include "core/input/input_structs.h"

UserInputInterface::UserInputInterface() { // Default constructor
    _listener = nullptr; // Initialize listener to null
    _id = -1; // Initialize ID to 0
}

UserInputInterface::~UserInputInterface() { // Destructor
    if (_listener != nullptr) {
        _listener->destroy(); // Destroy the listener if it exists
    }
}
void UserInputInterface::Init(InputListener *listener, int id) { // Initialize the interface with a listener and ID
    if(_listener != nullptr) {
        return; // If the listener is already initialized, do nothing
    }
    _listener = listener; // Set the listener
    _id = id; // Set the ID
}
bool UserInputInterface::getEvent(InputEvent &event) { // Get the event from the listener
    if (_listener != nullptr) {
        return _listener->getEvent(event); // Call the listener's getEvent method
    }
    return false;
}