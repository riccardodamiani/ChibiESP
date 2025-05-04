#ifndef GUI_EVENT_H
#define GUI_EVENT_H

enum class GuiEvent{
    NONE = 0,
    BUTTON_PRESSED = 1,
    CHANGED_FOCUS = 3,
    LIST_FOCUS_START = 4,
    LIST_FOCUS_END = 5
};

#endif //GUI_EVENT_H