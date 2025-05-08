#ifndef VIEW_H
#define VIEW_H

#include "core/graphics/gui_event.h"
#include "core/graphics/gui_element.h"
#include "core/graphics/view_error.h"
#include "core/graphics/view_render.h"

#include <string>
#include <vector>
#include <deque>
#include <mutex>
#include <atomic>

namespace{
    const int MAX_GUI_EVENT_COUNT = 10;
};

/**
 * @brief the view is the component that create, organize and update the gui element on screen. Also keep tracks of gui events generated
 */
class View{
public:
    View();
    ~View();

    //functions to update gui element state
    ViewError gui_list_add_text(const int elementId, const std::string &text);
    ViewError gui_set_text(const int elementId, const std::string &text);
    ViewError gui_get_Text(const int elementId, std::string &text) const;
    ViewError gui_list_select_text_index(const int elementId, const int index);
    ViewError gui_set_selectable(const int elementId, const bool selectable);
    ViewError gui_get_position(const int elementId, int &view_x, int& view_y);

    //functions to get gui events
    bool getEvent(GuiEvent& event);

    //functions to create new gui elements
    ViewError create_generic_element(const int elementId, std::string text, const int view_x, const int view_y);
    ViewError create_button_element(const int elementId, std::string text, const int view_x, const int view_y);
    ViewError create_list_element(const int elementId, const int view_x, const int view_y);

    //function to delete and element
    ViewError gui_delete_element(const int elementId);

    ViewError gui_get_selected_element(int &item);

    bool get_render_view(ViewRenderStruct &renderView);

    //INTERNAL USE ONLY
    //navigation functions: they run in the same thread of the user task
    void _gui_navigation_down(const int step);
    void _gui_navigation_up(const int step);
    void _gui_navigation_press();
    void _gui_navigation_release();
private:
    //element list incremental steps
    int gui_list_increment_text_index(CESP_GuiElement* element, const int step);
    int gui_list_decrement_text_index(CESP_GuiElement* element, const int step);

    //gui event stack functions
    int pushGuiEvent(GuiEvent);

    //some view private functions
    bool update_render_view();
    int findElement(const int elementId) const;
    int insertElementSorted(CESP_GuiElement* element);

    //private variables
    bool _requireViewUpdate;
    int _selectedElement;

    //element vector sorted by position on the view
    std::vector <CESP_GuiElement*> _elements;
    std::mutex _elementsMutex;

    //gui event queue
    std::deque <GuiEvent> _guiEvents;

    //stuff to draw
    ViewRenderStruct _drawFrame[2];
    std::atomic <uint8_t> _updateFrame;
    std::mutex _drawMutex;
};

#endif //VIEW_H