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