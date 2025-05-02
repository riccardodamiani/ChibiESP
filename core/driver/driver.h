#ifndef DRIVER_H
#define DRIVER_H

#include <string>

class CESP_Driver{
public:
    virtual int configure(void* arg);
    virtual int init(void* arg);
    virtual int deinit(void* arg);
    virtual int open(void* arg);
    virtual int close(void* arg);
    virtual int update(void* arg);
    virtual int read(void* arg);
    virtual int write(void* arg);
    virtual int seek(void* arg);
    virtual int get_device_info(void* arg);
    std::string get_name() const { return _name; }
protected:
    std::string _name; // Name of the driver
};

#endif  // DRIVER_H