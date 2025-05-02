#include "core/program/task_manager.h"
#include "core/program/task.h"

#include <map>
#include <mutex>

CESP_TaskManager::CESP_TaskManager(ChibiESP* kernelObj) : 
_kernel_obj(kernelObj)
{
    
}

void CESP_TaskManager::update(){

    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety
    // Delete terminated tasks
    for(auto& task : _task_map){
        CESP_Task* taskObj = task.second;
        CESP_TaskInfo_t taskInfo = taskObj->getInfo();
        if(taskInfo.status == CESP_TaskStatus::TASK_STATUS_TERMINATED){
            delete taskObj; // Delete the task object if terminated
            _task_map.erase(task.first); // Remove it from the map
            Logger::info("Task Manager: Task ID %d (%s) deleted", task.first, taskInfo.programName.c_str());
        }
    }
}

//create a new task and return the corresponding task ID
int CESP_TaskManager::create_new_task(const CESP_Program* const program){

    if(program == nullptr){
        Logger::error("Task Manager: Program is null, cannot create task");
        return -1; // Error: program is null
    }

    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety

    if(_task_map.size() >= 255){
        Logger::error("Task Manager: Maximum number of tasks reached. Cannot create task for %s", program->program_name.c_str());
        return -2; // Error: maximum number of tasks reached
    }

    //find the first taskId available
    uint8_t taskID = 0;
    for(auto& task : _task_map){
        if(_task_map.find(taskID) == _task_map.end()){  //map are always sorted for uint8_t key
            break; // Found an available task ID
        }
        ++taskID;
    }

    CESP_Task* task = new CESP_Task(_kernel_obj, _kernelCoreId, _userCoreId, program->program_name, taskID, 
        program->user_def_setup, program->user_def_loop, program->user_def_closeup);
    if(task == nullptr){
        Logger::error("Task Manager: Unable to create task object for %s", program->program_name.c_str());
        return -3; // Error: unable to create task object
    }
    _task_map[taskID] = task; // Store the task in the map

    Logger::info("Task Manager: Task ID %d (%s) created", taskID, program->program_name.c_str());
    return taskID;
    
}

//start a task by taskId
int CESP_TaskManager::start_task(const uint32_t taskID){
    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety

    if(_task_map.find(taskID) == _task_map.end()){
        return -1; // Error: task not found
    }

    CESP_Task* task = _task_map[taskID];
    task->start_task(); // Start the task

    Logger::info("Task Manager: Task ID %d (%s) started", taskID, task->getInfo().programName.c_str());
    return 0; // Task started successfully
}

//forcefully kill a task by taskId
int CESP_TaskManager::kill_task(const uint32_t taskID){

    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety
    if(_task_map.find(taskID) == _task_map.end()){
        return -1; // Error: task not found
    }

    CESP_Task* task = _task_map[taskID];
    if(task->getInfo().status == CESP_TaskStatus::TASK_STATUS_TERMINATING){
        return 0; // Task is already terminating
    }
    task->kill_task(); // request a task termination

    Logger::info("Task Manager: Requested task ID %d (%s) forced termination", taskID, task->getInfo().programName.c_str());
    return 0;
}

//Gacefully quit a task by taskId
int CESP_TaskManager::quit_task(const uint32_t taskID){

    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety
    if(_task_map.find(taskID) == _task_map.end()){
        return -1; // Error: task not found
    }

    CESP_Task* task = _task_map[taskID];
    if(task->getInfo().status == CESP_TaskStatus::TASK_STATUS_QUITTING){
        return 0; // Task is already terminating
    }
    task->quit_task(); // request a task termination

    Logger::info("Task Manager: Requested task ID %d (%s) graceful quit", taskID, task->getInfo().programName.c_str());
    return 0;
}

bool CESP_TaskManager::is_task_alive(const uint32_t taskID){
    std::lock_guard<std::mutex> lock(_task_map_mutex); // Lock the mutex for thread safety
    if(_task_map.find(taskID) == _task_map.end()){
        return false; // Error: task not found
    }
    return true;
}