#ifndef DRIVER_MANAGER_H
#define DRIVER_MANAGER_H

#include <vector>
#include <mutex>

#include "core/input/input_structs.h"

class ControlInputDriver;
class DisplayDriver;
class ChibiESP;

struct ControlInputControlStruct_t{
    ControlInputDriver* driver;
};

struct DisplayDriverControlStruct_t{
    DisplayDriver* driver;
};

class DriverManager {
public:
    int register_control_input_driver_module(ControlInputDriver* driver);
    int register_display_driver_module(DisplayDriver* driver);

    void init_control_input_drivers(void input_interrupt_callback(InputEvent &event));
    void init_display_drivers(ChibiESP* kernelInstance);

    //control input driver functions
    int update_control_input_drivers_state();

    //display driver functions
    DisplayDriver*  get_display_driver_by_name(std::string name);
private:
    //mutexes 
    std::mutex _displayDriverMutex;
    std::mutex _controlInputDriverMutex;

    //registered drivers
    std::vector <ControlInputControlStruct_t> _regControlInputDrivers; // List of input drivers registered
    std::vector <DisplayDriverControlStruct_t> _regDisplayDrivers; // List of drivers registered
};

#endif // DRIVER_MANAGER_H