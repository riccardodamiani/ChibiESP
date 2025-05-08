#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <stdint.h>
#include <string>
#include <vector>

#include "core/task/gui/view_error.h"
#include "core/task/gui/gui_event.h"

enum class Gui_ElementType{
    GENERIC = 0,    //no special functions
    BUTTON = 1,
    LIST = 2,
};

/**
 * * @brief internal class that implements a gui element. Each function is checked if applicable for the specific element
 */
class CESP_GuiElement{
public:
    CESP_GuiElement(uint16_t ID, Gui_ElementType type, std::string text, bool selectable, int view_pos_x, int view_pos_y);
    CESP_GuiElement(uint16_t ID, Gui_ElementType type, bool selectable, int view_pos_x, int view_pos_y);
    ViewError add_text(const std::string &new_string);
    ViewError setText(const std::string &new_string);
    ViewError getText(std::string &text) const;
    ViewError selectListTextIndex(int index);
    int incrementShownedText(const int step);
    int decrementShownedText(const int step);
    bool isSelectable() {return _selectable;}
    void setSelectable(bool selectable);
    int getX(){return _view_pos_x;}
    int getY(){return _view_pos_y;}

    bool _active;  //user is interacting with it
    const Gui_ElementType _element_type;
    const uint16_t _id;
private:
    int _view_pos_x, _view_pos_y;
    int16_t _display_text_index;
    std::vector <std::string> _text_list;
    bool _selectable; //user selected it
};

#endif  //GUI_ELEMENT_H