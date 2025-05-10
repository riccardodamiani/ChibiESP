#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <map>
#include <mutex>
#include <string>

class CESP_Task;
class ChibiKernel;
class CESP_Program;

class CESP_TaskManager{
public:
    CESP_TaskManager(ChibiKernel* kernelObj);
    void Init();
    void update();
    int create_new_task(const CESP_Program* const program);
    int start_task(const uint32_t taskID); // Start a task by ID
    int kill_task(const uint32_t taskID); // Kill a task by ID
    int quit_task(const uint32_t taskID);   // Quit a task by ID
    bool is_task_alive(const uint32_t taskID);   // checks whether a task is alive
    bool is_program_alive(const std::string programName);   // checks whether a task is alive
private:
    int _kernelCoreId; // ID of the kernel core
    int _userCoreId; // ID of the user core
    std::map<uint8_t, CESP_Task*> _task_map; // Map of task ID to task object

    std::mutex _task_map_mutex; // Mutex for thread safety

    ChibiKernel * const _kernel_obj;
};

#endif //TASK_MANAGER_H