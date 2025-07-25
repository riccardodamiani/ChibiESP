// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include "core/kernel/components/program_manager.h"
#include "core/structs/program.h"
#include "core/logging/logging.h"

bool CESP_ProgramManager::register_program(CESP_Program program){

    if(program.user_def_closeup == nullptr ||
        program.user_def_loop == nullptr ||
        program.user_def_setup == nullptr ||
        program.program_name == ""){
        Logger::error("Program Manager: Program has invalid parameters");
        return false;
    }

    std::lock_guard<std::mutex> lock(_program_mutex);

    // Check if the program is already registered
    for (const auto& existing_program : _programs) {
        if (existing_program->program_name == program.program_name) {
            Logger::error("Program Manager: Program %s is already registered", program.program_name.c_str());
            return false; // Program already registered
        }
    }

    // Register the new program
    _programs.push_back(new CESP_Program(program));

    Logger::info("Program Manager: Program %s registered", program.program_name.c_str());
    return true; // Success
}

bool CESP_ProgramManager::get_program_by_name(const std::string& name, CESP_Program*& program){
    std::lock_guard<std::mutex> lock(_program_mutex);

    for (const auto& prog : _programs) {
        if (prog->program_name == name) {
            program = prog;
            return true; // Program found
        }
    }
    return false; // Program not found
}