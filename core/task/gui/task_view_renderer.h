#ifndef TASK_VIEW_RENDERER_H
#define TASK_VIEW_RENDERER_H

#include "core/task/gui/view_render.h"

class DisplayDriver;

class TaskViewRenderer{
public:
    TaskViewRenderer();
    bool renderView(ViewRenderStruct &renderView);
private:
    DisplayDriver *_displayDriver;
    uint16_t _screenWidth, _screenHeight;

};

#endif