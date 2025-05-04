#include "core/graphics/view.h"
#include "core/graphics/gui_element.h"
#include "core/graphics/gui_event.h"
#include "core/graphics/view_error.h"
#include "core/logging/logging.h"

#include <mutex>
#include <atomic>
#include <string>

View::View(){
    _requireViewUpdate = false;
    _selectedElement = -1;
    _updateFrame = 0;
}

View::~View(){
    for(int i = 0; i < _elements.size(); i++){
        delete _elements[i];
    }
    _elements.clear();
}

/**
 * @brief add a text item to a gui list
 * @return return ViewError::NO_ERROR if the text was added, an error otherwise
 */
ViewError View::gui_list_add_text(const int elementId, const std::string &text){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available

    ViewError error = _elements[elementIndex]->add_text(text);
    if(error != ViewError::NO_ERROR) return error;
    _requireViewUpdate = true;
    return ViewError::NO_ERROR;
}

/**
 * @brief set the text of a GENERIC or BUTTON element
 * @return return ViewError::NO_ERROR if the text was changed correctly, an error otherwise
 * @details this function works only for GENERIC and BUTTON gui element. You cannot change text of a LIST element
 */
ViewError View::gui_set_text(const int elementId, const std::string &text){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available

    ViewError error = _elements[elementIndex]->setText(text);
    if(error != ViewError::NO_ERROR) return error;
    _requireViewUpdate = true;
    return ViewError::NO_ERROR;
}

/**
 * @brief get the text displayed by a gui element
 * @return a ViewError::NO_ERROR if the text was copied top text, an error otherwise
 * @details LISTs can have no text
 */
ViewError View::gui_get_Text(const int elementId, std::string &text) const{
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available

    ViewError error = _elements[elementIndex]->getText(text);
    if(error != ViewError::NO_ERROR) return error;
    return ViewError::NO_ERROR;
}

ViewError View::gui_list_select_text_index(const int elementId, const int index){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available

    return _elements[elementIndex]->selectListTextIndex(index);
}

ViewError View::gui_set_selectable(const int elementId, const bool selectable){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available
    _elements[elementIndex]->setSelectable(selectable);
    return ViewError::NO_ERROR;
}

ViewError View::gui_get_position(const int elementId, int &view_x, int&view_y){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;    //element not available

    view_x = _elements[elementIndex]->getX();
    view_y = _elements[elementIndex]->getY();
    return ViewError::NO_ERROR;
}

/**
 * @brief get a gui event
 * @param event a reference where the function will copy the new event
 * @return returns false if no event is present, true otherwise
 */
bool View::getEvent(GuiEvent& event){
    if(_guiEvents.size() == 0) return false;
    event = _guiEvents.front();
    _guiEvents.pop_front();
    return true;
}

int View::pushGuiEvent(GuiEvent event){

    if(_guiEvents.size() >= MAX_GUI_EVENT_COUNT) return -1;
    _guiEvents.push_back(event);
    return 0;
}


//functions to create new gui elements
ViewError View::create_generic_element(const int elementId, std::string text, const int view_x, const int view_y){
    int elementIndex = findElement(elementId);
    if(elementIndex >= 0) return ViewError::ITEM_ALREADY_EXISTS;    //element already exists

    CESP_GuiElement *new_element = new CESP_GuiElement(elementId, Gui_ElementType::GENERIC, text, false, view_x, view_y);
    int new_idem_index = insertElementSorted(new_element);

    //update selected element index to accomodate new element
    if(new_idem_index <= _selectedElement && _selectedElement >= 0) _selectedElement++;
    //if(_elements.size() == 1) _selectedElement = 0; //just to make sure _selectedElement is fine
    _requireViewUpdate = true;
    return ViewError::NO_ERROR;
}

ViewError View::create_button_element(const int elementId, std::string text, const int view_x, const int view_y){
    int elementIndex = findElement(elementId);
    if(elementIndex >= 0) return ViewError::ITEM_ALREADY_EXISTS;    //element already exists

    CESP_GuiElement *new_element = new CESP_GuiElement(elementId, Gui_ElementType::BUTTON, text, true, view_x, view_y);
    int new_idem_index = insertElementSorted(new_element);
    //Logger::info("New pos: %d", new_idem_index);
    //update selected element index to accomodate new element
    if(new_idem_index <= _selectedElement && _selectedElement >= 0) _selectedElement++;
    //if(_elements.size() == 1) _selectedElement = 0; //just to make sure _selectedElement is fine
    _requireViewUpdate = true;
    
    return ViewError::NO_ERROR;
}

ViewError View::create_list_element(const int elementId, const int view_x, const int view_y){
    int elementIndex = findElement(elementId);
    if(elementIndex >= 0) return ViewError::ITEM_ALREADY_EXISTS;    //element already exists

    CESP_GuiElement *new_element = new CESP_GuiElement(elementId, Gui_ElementType::LIST, true, view_x, view_y);
    int new_idem_index = insertElementSorted(new_element);

    //update selected element index to accomodate new element
    if(new_idem_index <= _selectedElement && _selectedElement >= 0) _selectedElement++;
    //if(_elements.size() == 1) _selectedElement = 0; //just to make sure _selectedElement is fine
    _requireViewUpdate = true;
    return ViewError::NO_ERROR;
}


int View::insertElementSorted(CESP_GuiElement* element) {
    auto it = _elements.begin();
    while (it != _elements.end()) {
        if ((*it)->getY() > element->getY()) break;
        if ((*it)->getY() == element->getY() && (*it)->getX() > element->getX()) break;
        ++it;
    }
    int index = it - _elements.begin();
    _elements.insert(it, element);
    return index;
}


//function to delete and element
ViewError View::gui_delete_element(const int elementId){
    int elementIndex = findElement(elementId);
    if(elementIndex < 0) return ViewError::ITEM_NOT_AVAILABLE;

    //delete the element
    delete _elements[elementIndex];
    _elements.erase(_elements.begin() + elementIndex);

    //the selected element index remains the same. The only case where is changed is if it goes out of range
    if(_elements.size() == 0){
        _selectedElement = -1;
    }else if(_selectedElement >= _elements.size()){
        _selectedElement = _elements.size() - 1;
    }

    _requireViewUpdate = true;
    return ViewError::NO_ERROR;
}

//element list incremental text change
int View::gui_list_increment_text_index(CESP_GuiElement* element, const int step){
    return element->incrementShownedText(step);
}

int View::gui_list_decrement_text_index(CESP_GuiElement* element, const int step){
    return element->decrementShownedText(step);
}

//navigation functions
void View::_gui_navigation_down(const int step){

    //the selected element has input focus
    if(_selectedElement >= 0 && _elements.size() > 0 && _elements[_selectedElement]->_active){
        CESP_GuiElement *selected_element = _elements[_selectedElement];
        if(selected_element->_element_type == Gui_ElementType::LIST){
            if(gui_list_decrement_text_index(selected_element, step) == 0){
                _requireViewUpdate = true;
            }
        }
        return;
    }

    //navigation between elements
    if(_elements.size() == 0){
        return;
    }

    int16_t new_selected = _selectedElement;
    for(int i = 0; i < step; i++){
        new_selected--;
        if(new_selected < 0) new_selected =  _elements.size() - 1;
        if(new_selected == _selectedElement){   //did a full round
            if(i == 0){  //there is nothing selectable
                new_selected = -1;
                break;
            }
        }
        if(!_elements[new_selected]->isSelectable()) i--;
    }
    _selectedElement = new_selected;

    //generate a gui event for the new element
    pushGuiEvent(GuiEvent::CHANGED_FOCUS);
    _requireViewUpdate = true;  //trigger a view update
}

void View::_gui_navigation_up(const int step){
    //the selected element has input focus
    if(_selectedElement >= 0 && _elements.size() > 0 && _elements[_selectedElement]->_active){
        CESP_GuiElement *selected_element = _elements[_selectedElement];
        if(selected_element->_element_type == Gui_ElementType::LIST){
            if(gui_list_increment_text_index(selected_element, step) == 0){
                _requireViewUpdate = true;
            }
        }
        return;
    }

    //navigation between elements
    if(_elements.size() == 0){
        return;
    }

    int16_t new_selected = _selectedElement;
    for(int i = 0; i < step; i++){
        new_selected++;
        if(new_selected >= _elements.size()) new_selected = 0;
        if(new_selected == _selectedElement){   //did a full round
            if(i == 0){  //there is nothing selectable
                new_selected = -1;
                break;
            }
        }
        if(!_elements[new_selected]->isSelectable()) i--;
    }
    _selectedElement = new_selected;

    //generate a gui event for the new element
    pushGuiEvent(GuiEvent::CHANGED_FOCUS);
    _requireViewUpdate = true;  //trigger a view update
}

/**
 * @brief called when the select button is released
 */
void View::_gui_navigation_activate(){
    if(_elements.size() == 0 || _selectedElement < 0 || _selectedElement >= _elements.size()) return;

    CESP_GuiElement *selected_element = _elements[_selectedElement];
    
    if(selected_element->_element_type == Gui_ElementType::BUTTON){
        pushGuiEvent(GuiEvent::BUTTON_PRESSED);
        return;
    }else if (selected_element->_element_type == Gui_ElementType::LIST){
        selected_element->_active = !selected_element->_active;
        if(selected_element->_active){
            pushGuiEvent(GuiEvent::LIST_FOCUS_START);
        }else{
            pushGuiEvent(GuiEvent::LIST_FOCUS_END);
        }
        _requireViewUpdate = true;  //trigger a view update
    }

}

/**
 * @brief return the id of the selected element
 * @return -1 if there is no element, the id of the selected element otherwise
 */
ViewError View::gui_get_selected_element(int &item){
    if(_elements.size() == 0 || _selectedElement < 0 || _selectedElement >= _elements.size()) return ViewError::ITEM_NOT_AVAILABLE;
    item = _elements[_selectedElement]->_id;
    return ViewError::NO_ERROR;
}

int View::findElement(const int elementId) const{
    for(int index = 0; index < _elements.size(); index++){
        CESP_GuiElement* element = _elements[index];
        if(element->_id == elementId){
            return index;
        }
    }
    return -1;
}

/**
 * @brief updates the view render data
 * @return true if the render data was updated, false if nothing changed
 */
bool View::update_render_view(){
    if(!_requireViewUpdate) return false; //nothing to update

    //std::lock_guard <std::mutex> lock(_drawMutex);
    ViewRenderStruct &updateFrame =  _drawFrame[_updateFrame];
    updateFrame.elements.clear();
    updateFrame.focusedItemIndex = _selectedElement;

    for(auto& item : _elements){
        ItemRenderStruct render_item;
        render_item.height = 0;
        render_item.real_x = 0;
        render_item.real_y = 0;
        render_item.text = "";
        render_item.width = 0;
        render_item.view_x = item->getX();
        render_item.view_y = item->getY();
        item->getText(render_item.text);
        updateFrame.elements.push_back(render_item);
    }
    _requireViewUpdate = false;
    return true;
}

/**
 * @brief get the render data from the view
 * @details 
 */
bool View::get_render_view(ViewRenderStruct &renderView){
    if(update_render_view()){
        _updateFrame = 1 - _updateFrame;    //exchange the frames        
    }
    renderView = _drawFrame [1 - _updateFrame]; //get current drawable frame
    return true;
}