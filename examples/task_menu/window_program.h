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

const void window_program_setup(CESP_UserTaskData &taskData){
    Logger::info("Window program setup");

    taskData.taskInterface.createView();
    View *view = taskData.taskInterface.getActiveView();
    view->create_generic_element(0, "Sample window", 0, 0);
    view->create_button_element(1, "close window", 0, 10);
}

const void window_program_loop(CESP_UserTaskData &taskData){
    InputEvent event;
    while(taskData.taskInterface.getInputEvent(event)){
  
    }
    View* view = taskData.taskInterface.getActiveView();
    if(view != nullptr){
      GuiEvent gui_event;
      while(view->getEvent(gui_event)){
        Logger::info("New gui event: %d", static_cast<int>(gui_event));

        // Handle button press events
        switch(gui_event){
          case GuiEvent::BUTTON_PRESSED:
          {
            // Check if the close window button was pressed
            int elementId;
            ViewError returnCode = view->gui_get_selected_element(elementId);
            if(returnCode == ViewError::NO_ERROR && elementId == 1){ // Close window button pressed
              chibiESP.quitTask(taskData.taskID); // Gracefully quit the task
            }
            break;
          }
          default:
            break;
        }
      }
    }
    delay(10);
}

const void window_program_closeup(CESP_UserTaskData &task){
    Logger::info("Closing window program");
}
