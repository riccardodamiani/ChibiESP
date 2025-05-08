#ifndef TASK_VIEW_RENDERER_H
#define TASK_VIEW_RENDERER_H

#include "core/task/gui/view_render.h"

class DisplayDriver;
class ChibiESP;

class TaskViewRenderer{
public:
    TaskViewRenderer(ChibiESP*);
    bool renderView(ViewRenderStruct &renderView);
private:
    DisplayDriver *_displayDriver;

};

#endif