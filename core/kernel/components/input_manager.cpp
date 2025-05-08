#include "core/kernel/components/input_manager.h"
#include "core/kernel/components/input_listener.h"
#include "core/logging/logging.h"

/**
 * @brief Creates a new input listener and registers it with the input manager.
 * * @param listener The input listener to be created and registered.
 * * @return The ID of the newly created input listener.
 */
int InputManager::createInputListener(InputListener *&listener){

    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    //find the first listener id available
    uint16_t listenerId = 0;
    for(auto& listener : _inputListeners){
        if(_inputListeners.find(listenerId) == _inputListeners.end()){  //map are always sorted for uint16_t key
            break; // Found an available listener ID
        }
        ++listenerId;
        if(listenerId >= 500){
            Logger::error("InputManager: Too many input listeners");
            return -1; // Error: no available listener ID
        }
    }

    InputListener *new_listener = new InputListener();
    _inputListeners[listenerId] = new_listener;
    listener = new_listener; // Assign the listener to the provided pointer
    Logger::info("InputManager: Listener ID %d created", listenerId); // Log the creation of the listener
    return listenerId; // Return the ID of the newly created listener    
}

void InputManager::update(){
    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    for (auto it = _inputListeners.begin(); it != _inputListeners.end(); ) {
        InputListener *listener = it->second;
        if (!listener->isAlive()) { // Verifica se il listener non è vivo
            Logger::info("InputManager: Removing dead listener ID %d", it->first);
            delete listener;
            it = _inputListeners.erase(it); // Rimuovi il listener e aggiorna l'iteratore
        } else {
            ++it; // Passa al prossimo elemento
        }
    }
}

void InputManager::input_interrupt_callback(InputEvent &event){
    dispatchEvent(event); // Dispatch the event to registered destinations
}


void InputManager::dispatchEvent(InputEvent event){
    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    for(auto it = _inputListeners.begin(); it != _inputListeners.end(); it++) {
        InputListener *listener = it->second;
        listener->pushEvent(event);
    }
}