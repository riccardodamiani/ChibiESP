#ifndef PROGRAM_H
#define PROGRAM_H

#include "core/task/user_task.h"

#include <string>

class CESP_Program{
public:
    CESP_Program( const std::string& program_name,
        const void (*user_def_setup)(CESP_UserTaskData& data), 
        const void (*user_def_loop)(CESP_UserTaskData& data),
        const void (*user_def_closeup)(CESP_UserTaskData& data)) :
        program_name(program_name),
        user_def_setup(user_def_setup),
        user_def_loop(user_def_loop),
        user_def_closeup(user_def_closeup)
    {
    };
    const void (*user_def_setup)(CESP_UserTaskData& data);
    const void (*user_def_loop)(CESP_UserTaskData& data); 
    const void (*user_def_closeup)(CESP_UserTaskData& data);
    const std::string program_name;
};

#endif //PROGRAM_H