// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#include <chibiESP.h>
#include <core/logging/logging.h>
#include <core/structs/program.h>

#include "wifi_program.h"

#include <vector>
#include <memory>

void user_setup_function(){
  CESP_Program wifiProgram("wifi program", wifi_program_setup, wifi_program_loop, wifi_program_closeup );

  chibiESP.createProgram(wifiProgram);

  int programId = chibiESP.startProgram("wifi program");
  if(programId < 0){
    Logger::error("Failed to start wifi program");
  }else{
    Logger::info("Started wifi program with ID: %d", programId);
  }
}

void setup() {
  chibiESP.init();

  //initialize the system devices
  chibiESP.init_kernel_devices();

  //start the user setup function
  user_setup_function();
}

void loop() {
  chibiESP.loop();

  delay(10);
}