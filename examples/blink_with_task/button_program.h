#pragma once

#include <core/task/task_memory.h>
#include <core/task/user_task.h>
#include <core/logging/logging.h>
#include <chibiESP.h>

#include <memory>

#define START_TASK_BUTTON_ID 1
#define STOP_TASK_BUTTON_ID 0

class ButtonProgram_Memory : public CESP_TaskMemory{
    public:
      int prog2_taskID;
};

const void button_program_setup(CESP_UserTaskData &taskData){
    Logger::info("Button program started");

    taskData.userDataPtr = std::unique_ptr<ButtonProgram_Memory>(new ButtonProgram_Memory());
    ButtonProgram_Memory* memory = static_cast<ButtonProgram_Memory*>(taskData.userDataPtr.get());
    memory->prog2_taskID = -1;
}

const void button_program_loop(CESP_UserTaskData &taskData){
    InputEvent event;
    while(taskData.taskInterface.getInputEvent(event)){
      if(event.type == InputEventType::INPUT_EVENT_KEY && 
        static_cast<KeyEventType>(event.deviceEventType) == KeyEventType::KEY_EVENT_RELEASED){
        ButtonProgram_Memory* memory = static_cast<ButtonProgram_Memory*>(taskData.userDataPtr.get());
        if(event.deviceID == START_TASK_BUTTON_ID){
          if(chibiESP.isProgramRunning("blink program")){
            Logger::info("Blink task already running");
          }else{
            Logger::info("Starting blink program");
            memory->prog2_taskID = chibiESP.startProgram("blink program");
          }
        }else if(event.deviceID == STOP_TASK_BUTTON_ID){
          if(!chibiESP.isProgramRunning("blink program")){
            Logger::info("Blink task is not running");
          }else{
            Logger::info("Quitting blink program");
            chibiESP.quitTask(memory->prog2_taskID);
          }
        }
      }
    }
    delay(50);
}

const void button_program_closeup(CESP_UserTaskData &task){
    Logger::info("Closing button program");
}
