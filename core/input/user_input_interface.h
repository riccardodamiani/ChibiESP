#ifndef USER_INPUT_INTERFACE_H
#define USER_INPUT_INTERFACE_H

#include "core/input/input_structs.h"   //for InputEvent

class InputListener;

class UserInputInterface{
    public:
        UserInputInterface();
        ~UserInputInterface();
        void Init(InputListener *listener, int id);
        bool getEvent(InputEvent &event);
    private:
        InputListener *_listener; // Pointer to the input listener
        int _id; // ID of the input listener
    };

#endif //USER_INPUT_INTERFACE_H