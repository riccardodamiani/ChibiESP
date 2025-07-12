// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef TASK_VIEW_RENDERER_H
#define TASK_VIEW_RENDERER_H

#include "core/task/gui/view_render.h"

class DisplayDevice;

class TaskViewRenderer{
public:
    TaskViewRenderer();
    bool renderView(ViewRenderStruct &renderView);
private:
    DisplayDevice *_displayDevice;
    uint16_t _screenWidth, _screenHeight;

};

#endif