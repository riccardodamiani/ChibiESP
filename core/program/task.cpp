#include "core/program/task.h"
#include "core/program/task_memory.h"
#include "core/program/user_task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <atomic>

CESP_Task::CESP_Task(const int kernelCoreId, const int userCoreId, const std::string& programName, const uint32_t taskID, 
    const void (*user_def_setup)(CESP_UserTaskData&), 
    const void (*user_def_loop)(CESP_UserTaskData&), 
    const void (*user_def_closeup)(CESP_UserTaskData&)) :
    _taskInfo(programName, taskID, kernelCoreId, userCoreId, user_def_setup, user_def_loop, user_def_closeup) // Initialize task status
{
    _taskInfo.userDataPtr = nullptr; // Initialize user data pointer to null

    _taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_IDLE; // Set initial task status to idle
    _taskStatus.task_start_time = millis();
    _taskStatus.terminationRequest = false;
    _taskStatus.user_task_terminated = false; // Initialize user task terminated flag
}

void CESP_Task::start_task(){

    //prepare the task memory
    CESP_UserTaskData userTaskData = {_taskInfo.programName, _taskInfo.task_id, _taskInfo.userDataPtr};
    InternalTaskFullData_t *taskData = new InternalTaskFullData_t(this, userTaskData, _taskInfo, _taskStatus);

    _taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_RUNNING; // Set task status to running

    //starts the two ESP tasks in different cores
    xTaskCreatePinnedToCore(monitorTaskWrapper, "MonitorLoop", 4096, taskData, 1, &_taskStatus.monitorLoopHandle, _taskInfo.kernelCoreId);
    xTaskCreatePinnedToCore(userTaskWrapper, "UserLoop", 4096, taskData, 1, &_taskStatus.userLoopHandle, _taskInfo.userCoreId);
}

void CESP_Task::kill_task(){
    _taskStatus.terminationRequest = true; // Set termination request flag
}

void CESP_Task::user_task_function(void *args){
    InternalTaskFullData_t *taskData = static_cast<InternalTaskFullData_t*>(args);
    CESP_UserTaskData &userTaskData = taskData->userTaskData;
    InternalTaskInfo_t &taskInfo = taskData->taskInfo;
    InternalTaskStatus_t &taskStatus = taskData->taskStatus;

    // Call user-defined setup function
    if (taskInfo.user_def_setup) {
        taskInfo.user_def_setup(userTaskData);
    }

    // Call user-defined loop function
    if (taskInfo.user_def_loop) {
        while(!taskStatus.terminationRequest) {
            taskInfo.user_def_loop(userTaskData);
            taskStatus.task_user_function_start_time = millis(); // Update the start time of the user function
        }
    }
    taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_TERMINATING; // Set task status to terminated

    // Call user-defined closeup function
    if (taskInfo.user_def_closeup) {
        taskInfo.user_def_closeup(userTaskData);
    }

    delay(10);
    taskStatus.user_task_terminated = true;
    vTaskDelete(taskStatus.userLoopHandle); // Delete the user loop task
}

void CESP_Task::monitor_task_function(void *args){
    while(true){
        InternalTaskFullData_t *taskData = static_cast<InternalTaskFullData_t*>(args);
        CESP_UserTaskData &userTaskData = taskData->userTaskData;
        InternalTaskInfo_t &taskInfo = taskData->taskInfo;
        InternalTaskStatus_t &taskStatus = taskData->taskStatus;

        if(taskStatus.user_task_terminated){
            taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_TERMINATED; // Set task status to terminated
            TaskHandle_t currentTask = taskStatus.monitorLoopHandle;
            delete taskData; // Delete the task data
            vTaskDelete(currentTask); // Delete the monitor loop task
            return; // Exit the monitoring loop
        }

        //check whether the user mode task is still responding
        if(millis() - taskStatus.task_user_function_start_time > 1000){
            taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_NOT_RESPONDING; // Set task status to not responding
        } else {
            if(taskStatus.task_status == CESP_TaskStatus::TASK_STATUS_NOT_RESPONDING){
                taskStatus.task_status = CESP_TaskStatus::TASK_STATUS_RUNNING; // Set task status to running
            }
        }

        delay(50);
    }
}

CESP_TaskInfo_t CESP_Task::getInfo() const{
    CESP_TaskInfo_t info;
    info.programName = _taskInfo.programName;
    info.status = _taskStatus.task_status.load(); // Use atomic load for thread safety
    info.taskID = _taskInfo.task_id;
    info.task_alive_time = millis() - _taskStatus.task_start_time; // Calculate alive time in milliseconds
    return info;
}
