// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#pragma once

#include <chibiESP.h>
#include <core/logging/logging.h>
#include <core/task/user_task.h>
#include <core/task/gui/view.h>
#include <core/task/gui/gui_event.h>
#include <core/structs/input_structs.h>

#include <memory>

const void menu_program_setup(CESP_UserTaskData &taskData){
    Logger::info("Menu program setup");

    taskData.taskInterface.createView();
    View *view = taskData.taskInterface.getActiveView();
    view->create_generic_element(0, "this is a text", 0, 0);
    view->create_button_element(1, "button", 0, 10);
    view->create_generic_element(2, "try this:", 0, 20);
    view->create_list_element(3, 60, 20);
    view->gui_list_add_text(3, "option 1");
    view->gui_list_add_text(3, "option 2");
    view->gui_list_add_text(3, "option 3");
    view->gui_list_add_text(3, "option 4");
    view->create_generic_element(4, "press: ", 0, 30);
    view->create_button_element(5, "button2", 40, 30);
    view->create_button_element(6, "button3", 0, 40);
}

const void menu_program_loop(CESP_UserTaskData &taskData){
    InputEvent event;
    while(taskData.taskInterface.getInputEvent(event)){
  
    }
    View* view = taskData.taskInterface.getActiveView();
    if(view != nullptr){
      GuiEvent gui_event;
      while(view->getEvent(gui_event)){
        Logger::info("New gui event: %d", static_cast<int>(gui_event));
      }
    }
    delay(10);
}

const void menu_program_closeup(CESP_UserTaskData &task){
    Logger::info("Closing menu program");
}
