// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/task/task_interface.h"
#include "core/kernel/components/input_listener.h"
#include "core/task/gui/view_render.h"
#include "core/task/gui/view.h"
#include "core/task/gui/task_view_renderer.h"
#include "chibiESP.h"

TaskInterface::TaskInterface(bool enableGraphics, InputListener *listener) : 
_enableGraphics(enableGraphics)
{
    _inputListener = nullptr; // Initialize listener to null
    _deleteCurrentView = false;

    inputInit(listener);
    _viewRenderer = new TaskViewRenderer();
    _upNavEvent = chibiESP.getNavUpEvent();
    _downNavEvent = chibiESP.getNavDownEvent();
    _selectNavEvent = chibiESP.getNavSelectEvent();
    _renderTimer = millis();
}

TaskInterface::~TaskInterface() { // Destructor
    if (_inputListener != nullptr) {
        _inputListener->destroy(); // Destroy the listener if it exists. Delete is called by the kernel
    }

    for(int i = 0; i < _views.size(); i++){
        delete _views[i];
    }
    _views.clear();
    delete _viewRenderer;
}

// Initialize the interface with a listener and ID. Internal use only.
void TaskInterface::inputInit(InputListener *listener) {
    if(_inputListener != nullptr) {
        return; // If the listener is already initialized, do nothing
    }
    _inputListener = listener; // Set the listener
}

bool TaskInterface::getInputEvent(InputEvent &event) { // Get the event from the listener
    if (_inputListener == nullptr) {
        return false;
    }

    InputEvent new_event;
    bool event_avail = _inputListener->getEvent(new_event); // Call the listener's getEvent method
    if(!event_avail) return false;

    //dispatch the navigation command to the view
    if(_enableGraphics && _views.size() > 0){
        View* active_view = _views.back();

        //checks up navigation event
        if(new_event.type == _downNavEvent.type &&
            new_event.deviceID == _downNavEvent.deviceID &&
            new_event.deviceEventType == _downNavEvent.deviceEventType &&
            ((new_event.type == InputEventType::INPUT_EVENT_WHEEL && new_event.eventData > 0) ||
            new_event.type == InputEventType::INPUT_EVENT_KEY)){
            int step = new_event.type == InputEventType::INPUT_EVENT_KEY ? 1 : abs(new_event.eventData);
            active_view->_gui_navigation_up(step);
        }

        //checks down navigation event
        if(new_event.type == _downNavEvent.type &&
            new_event.deviceID == _downNavEvent.deviceID &&
            new_event.deviceEventType == _downNavEvent.deviceEventType &&
            ((new_event.type == InputEventType::INPUT_EVENT_WHEEL && new_event.eventData < 0) ||
            new_event.type == InputEventType::INPUT_EVENT_KEY)){
            int step = new_event.type == InputEventType::INPUT_EVENT_KEY ? 1 : abs(new_event.eventData);
            active_view->_gui_navigation_down(step);
        }

        //checks select navigation event
        if(new_event.type == _selectNavEvent.type &&
            new_event.deviceID == _selectNavEvent.deviceID &&
            new_event.deviceEventType == static_cast <uint8_t>(KeyEventType::KEY_EVENT_RELEASED)){
            active_view->_gui_navigation_release();
        }    
        if(new_event.type == _selectNavEvent.type &&
            new_event.deviceID == _selectNavEvent.deviceID &&
            new_event.deviceEventType == static_cast <uint8_t>(KeyEventType::KEY_EVENT_PRESSED)){
            active_view->_gui_navigation_press();
        }          
    }

    //return the event
    event = new_event;
    return true;
}

void TaskInterface::clearInputs(){
    if (_inputListener != nullptr) {
        return _inputListener->clearEvents(); // Call the listener's getEvent method
    }
}

//graphical functions
View* TaskInterface::getActiveView(){
    if(!_enableGraphics || _views.size() == 0){
        return nullptr;
    }

    return _views.back();
}

bool TaskInterface::deleteCurrentView(){
    if(!_enableGraphics){
        return false;
    }

    if(_views.size() > 0){
        _deleteCurrentView = true;  //the next update the view will be destroyed
    }
    return _deleteCurrentView;
}

bool TaskInterface::createView(){
    if(!_enableGraphics){
        return false;
    }

    //std::lock_guard <std::mutex> lock(_viewMutex);
    _views.push_back(new View());
    return true;
}

void TaskInterface::_updateInterface(){
    //std::lock_guard <std::mutex> lock(_viewMutex);
    if(!_enableGraphics || _views.size() == 0){
        return;
    }

    if(_deleteCurrentView){
        View* to_erase = _views.back();
        delete to_erase;
        _views.pop_back();
        _deleteCurrentView = false;
    }

    if(millis() - _renderTimer > 100){
        _renderTimer = millis();
        renderView();
    }
}

bool TaskInterface::renderView(){
    if(!_enableGraphics || _views.size() == 0){
        return false;
    }

    //std::lock_guard <std::mutex> lock(_viewMutex);
    View* active_view = _views.back();
    ViewRenderStruct viewRender;
    bool ret = active_view->get_render_view(viewRender);
    if(!ret) return false;
    return _viewRenderer->renderView(viewRender);
}