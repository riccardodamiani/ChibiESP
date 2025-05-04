#ifndef TASK_H
#define TASK_H

#include "core/program/task_memory.h"
#include "core/program/user_task.h"
#include "core/input/user_input_interface.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <string>
#include <memory>
#include <atomic>

class ChibiESP;
class TaskInterface;

//public task status enum
enum class CESP_TaskStatus{
    TASK_STATUS_IDLE = 0, // Task not running
    TASK_STATUS_RUNNING = 1, // Task is running
    TASK_STATUS_TERMINATED = 2, // Task killed
    TASK_STATUS_TERMINATING = 3, // Task is terminating
    TASK_STATUS_QUITTING = 4, // Task is quitting
    TASK_STATUS_NOT_RESPONDING = 5 // Task not responding
};

//public information structure about the task
struct CESP_TaskInfo_t{
    std::string programName;
    CESP_TaskStatus status;
    uint32_t taskID; // ID of the task
    uint32_t task_alive_time; // Time the task has been alive in milliseconds
};

/**
 * @brief CESP_Task class for managing tasks in the ChibiESP framework. Internal use only.
 * This class contains and manage a task (chibiESP process). When the task is started two ESP tasks are created:
 * one on the user core that runs the user define code, and one in kernel core that monitors the task.
 */
class CESP_Task{
public:
//public methods
    CESP_Task(ChibiESP* kernelObj, const int kernelCoreId, const int userCoreId, const std::string& programName, const uint32_t taskID, 
        const void (*user_def_setup)(CESP_UserTaskData& data), 
        const void (*user_def_loop)(CESP_UserTaskData& data), 
        const void (*user_def_closeup)(CESP_UserTaskData& data));
    ~CESP_Task();
    CESP_TaskInfo_t getInfo() const; // Get task information

    void start_task(); // Start the task
    void user_task_function(void *args); // Task loop function
    void monitor_task_function(void *args); // Monitor the task
    void kill_task();
    void quit_task();

    static void monitorTaskWrapper(void* arg){
        InternalTaskFullData_t* task = static_cast<InternalTaskFullData_t*>(arg);
        task->taskObj->monitor_task_function(arg);
    }
    static void userTaskWrapper(void* arg){
        InternalTaskFullData_t* task = static_cast<InternalTaskFullData_t*>(arg);
        task->taskObj->user_task_function(arg);
    }

private:
    ChibiESP* const _kernelObj;
    TaskInterface* _taskInterface;

//privare structure contasining all task information
struct InternalTaskInfo_t{
    const std::string programName;
    const uint32_t task_id; // ID of the task
    const int kernelCoreId; // Core ID for task monitoring
    const int userCoreId; // Core ID for user mode
    const void (*user_def_setup)(CESP_UserTaskData& data); // User-defined setup function
    const void (*user_def_loop)(CESP_UserTaskData& data); // User-defined loop function
    const void (*user_def_closeup)(CESP_UserTaskData& data); // User-defined closeup function
    std::unique_ptr<CESP_TaskMemory> userDataPtr; // Pointer to user data

    // Costruttore personalizzato
    InternalTaskInfo_t(const std::string& name, uint32_t id, const int kernelCore, const int userCore,
        const void (*setup)(CESP_UserTaskData&), const void (*loop)(CESP_UserTaskData&),
        const void (*closeup)(CESP_UserTaskData&)) : 
            programName(name), task_id(id), kernelCoreId(kernelCore), userCoreId(userCore),
            user_def_setup(setup), user_def_loop(loop), user_def_closeup(closeup) {}
}_taskInfo;

struct InternalTaskStatus_t{
    std::atomic <CESP_TaskStatus> task_status;
    uint32_t task_start_time; // Start time of the task
    std::atomic <uint32_t> task_user_function_start_time; // Start time of the user function
    std::atomic <bool> terminationRequest; // Flag for task forced termination request
    std::atomic <bool> quitRequest; // Flag for task graceful quit request
    std::atomic <bool> user_task_terminated; // Flag for task termination
    TaskHandle_t monitorLoopHandle; // Handle for the task monitor loop
    TaskHandle_t userLoopHandle;    // Handle for the user 
}_taskStatus;

struct InternalTaskFullData_t{
    CESP_Task *taskObj; // Task object
    CESP_UserTaskData userTaskData; // User task data
    InternalTaskInfo_t &taskInfo; // Task information
    InternalTaskStatus_t &taskStatus; // Task status

    // Costruttore personalizzato
    InternalTaskFullData_t(CESP_Task* task, CESP_UserTaskData& userData, 
        CESP_Task::InternalTaskInfo_t& info, 
        CESP_Task::InternalTaskStatus_t& status) :
        taskObj(task), userTaskData(userData), taskInfo(info), taskStatus(status) {}
};

};

#endif //TASK_H