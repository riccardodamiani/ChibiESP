#ifndef USER_TASK_H
#define USER_TASK_H

#include <string>
#include <memory>

#include "core/program/task_memory.h"
#include "core/input/user_input_interface.h"

//data structure the user custom functions receive as argument
struct CESP_UserTaskData{
    const std::string programName; // Name of the task
    const uint32_t taskID; // ID of the task
    std::unique_ptr<CESP_TaskMemory> &userDataPtr; // Pointer to user data
    UserInputInterface &input; // Input listener for the task
};

#endif //USER_TASK_H