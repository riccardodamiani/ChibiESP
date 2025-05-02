#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "core/program/program.h"
#include "core/program/task.h"

#include <map>
#include <mutex>

class CESP_TaskManager{
public:
    void update();
    int create_new_task(const CESP_Program* const program);
    int start_task(const uint32_t taskID); // Start a task by ID
    int kill_task(const uint32_t taskID); // Kill a task by ID
    int quit_task(const uint32_t taskID);   // Quit a task by ID
    bool is_task_alive(const uint32_t taskID);   // checks whether a task is alive
private:
    int _freeTaskId; // ID of the next created task
    int _kernelCoreId; // ID of the kernel core
    int _userCoreId; // ID of the user core
    std::map<uint8_t, CESP_Task*> _task_map; // Map of task ID to task object

    std::mutex _task_map_mutex; // Mutex for thread safety
};

#endif //TASK_MANAGER_H