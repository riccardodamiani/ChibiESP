// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef GUI_EVENT_H
#define GUI_EVENT_H

enum class GuiEvent{
    NONE = 0,
    BUTTON_PRESSED = 1,
    BUTTON_RELEASED = 2,
    CHANGED_FOCUS = 3,
    LIST_FOCUS_START = 4,
    LIST_FOCUS_END = 5
};

#endif //GUI_EVENT_H