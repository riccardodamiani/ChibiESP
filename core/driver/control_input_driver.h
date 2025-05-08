#ifndef CONTROL_INPUT_DRIVER_H
#define CONTROL_INPUT_DRIVER_H

#include "core/input/input_structs.h"
#include <string>

struct ControlDriverInitStruct_t{
    void (*input_interrupt)(InputEvent&);
};

class ControlInputDriver {
public:
    ControlInputDriver();
    virtual int configure(void* arg);
    virtual int init(ControlDriverInitStruct_t& init_struct);
    virtual int deinit();
    virtual int update();
    virtual int get_device_info(void* arg);
    std::string get_name() const { return _name; }
protected:
    std::string _name;

};

#endif // CONTROL_INPUT_DRIVER_H