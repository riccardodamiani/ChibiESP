#ifndef TASK_INTERFACE_H
#define TASK_INTERFACE_H

#include "core/structs/input_structs.h"   //for InputEvent
#include "core/task/gui/view_render.h"

#include <vector>
#include <mutex>

class ChibiESP;
class InputListener;
class View;
class TaskViewRenderer;

/**
 * @brief task component that handles inputs and view elements for the task
 */
class TaskInterface{
public:
    TaskInterface(bool enableGraphics, ChibiESP* kernelInstance, InputListener *listener);
    ~TaskInterface();

    //input functions
    bool getInputEvent(InputEvent &event);
    void clearInputs();

    //graphical functions
    View* getActiveView();
    bool deleteCurrentView();
    bool createView();

    //Internal use only functions  
    void _updateInterface();
private:
    void inputInit(InputListener *listener);
    bool renderView();

    //input variables
    InputListener *_inputListener;
    TaskViewRenderer *_viewRenderer;

    //view variabiles
    const bool _enableGraphics; //disables all graphical functionality
    std::vector <View *> _views;
    bool _deleteCurrentView;
    std::mutex _viewMutex;
    int _renderTimer;

    //navigation input events
    InputEvent _upNavEvent, _downNavEvent, _selectNavEvent;
};

#endif//TASK_INTERFACE_H