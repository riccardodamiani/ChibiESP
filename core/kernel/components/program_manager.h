// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#ifndef PROGRAM_MANAGER_H
#define PROGRAM_MANAGER_H

#include "core/structs/program.h"

#include <string>
#include <vector>
#include <mutex>

class CESP_ProgramManager{
public:
    bool register_program(CESP_Program program); // Register a new program
    bool get_program_by_name(const std::string& name, CESP_Program*& program); // Get a program by name
private:
    std::vector<CESP_Program*> _programs; // List of programs registered
    std::mutex _program_mutex; // Mutex for thread safety
};

#endif //PROGRAM_MANAGER_H