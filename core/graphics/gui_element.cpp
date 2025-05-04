#include "core/graphics/gui_element.h"
#include "core/graphics/view_error.h"

/**
 * @brief constructor used for every element except lists. Also include a text
 */
CESP_GuiElement::CESP_GuiElement(const uint16_t id, Gui_ElementType type, std::string text, bool selectable, int view_pos_x, int view_pos_y) :
    _element_type(type),
    _id(id)
{
    _active = false;
    _display_text_index = -1;
    _view_pos_x = view_pos_x;
    _view_pos_y = view_pos_y;
    //everything other than a list should always have a text
    if(_element_type != Gui_ElementType::LIST){
        _text_list.push_back(text);
    }
    _selectable = selectable;
}

/**
 * @brief constructor used for lists: it doesn't provide the text
 */
CESP_GuiElement::CESP_GuiElement(uint16_t id, Gui_ElementType type, bool selectable, int view_pos_x, int view_pos_y) :
    _element_type(type),
    _id(id)
{
    _active = false;
    _display_text_index = -1;
    _view_pos_x = view_pos_x;
    _view_pos_y = view_pos_y;

    //everything other than a list should always have a text
    if(_element_type != Gui_ElementType::LIST){
        _text_list.push_back("");
    }
    _selectable = selectable;
}

ViewError CESP_GuiElement::add_text(const std::string &new_string){
    if(_element_type != Gui_ElementType::LIST)
        return ViewError::FUNCTION_NOT_AVAILABLE;  //invalid function for this element

    _text_list.push_back(new_string);
    if(_text_list.size() == 1){
        _display_text_index = 0;
    }
    return ViewError::NO_ERROR;
}

ViewError CESP_GuiElement::setText(const std::string &new_string){
    if(_element_type == Gui_ElementType::LIST)
        return ViewError::FUNCTION_NOT_AVAILABLE;  //invalid function for this element

    _text_list[0] = new_string;
    ViewError::NO_ERROR;
}

ViewError CESP_GuiElement::getText(std::string &text) const{
    if(_element_type == Gui_ElementType::LIST){
        if(_text_list.size() == 0) return ViewError::EMPTY_FIELD;   //nothing available
        text = _text_list[_display_text_index]; //return current displayed string
        return ViewError::NO_ERROR;
    }
    text = _text_list[0];
    return ViewError::NO_ERROR;
}

ViewError CESP_GuiElement::selectListTextIndex(int index){
    if(_element_type != Gui_ElementType::LIST)
        return ViewError::FUNCTION_NOT_AVAILABLE;  //invalid function for this element

    if(index < 0 || index > _text_list.size()){
        return ViewError::INVALID_PARAM; //invalid parameter
    }

    _display_text_index = index;
    return ViewError::NO_ERROR;
}

int CESP_GuiElement::incrementShownedText(const int step){
    if(_element_type != Gui_ElementType::LIST)
        return -1;  //invalid function for this element

    //increment and wrap around
    if(_text_list.size() < 2) return -2;    //nothing to change
    _display_text_index += step;
    _display_text_index %= _text_list.size();
    return 0;
}

int CESP_GuiElement::decrementShownedText(const int step){
    if(_element_type != Gui_ElementType::LIST)
        return -1;  //invalid function for this element

    //decrement and wrap around
    if(_text_list.size() < 2) return -2;    //nothing to change
    _display_text_index -= step;

    if(_display_text_index < 0){
        _display_text_index = abs(_display_text_index);
        _display_text_index %= _text_list.size();
        _display_text_index =  _text_list.size() - _display_text_index;
    }
    return 0;
}

void CESP_GuiElement::setSelectable(bool selectable){
    _selectable = selectable;
}
