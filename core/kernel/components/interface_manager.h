#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include <map>

class TwoWire;
class I2cBus;

class InterfaceManager{
public:
    InterfaceManager() = default;
    bool registerI2cInterface(int bus, int sda_pin, int scl_pin);
    TwoWire* getI2cInterface(int bus);
private:
    std::map <uint8_t, I2cBus*> _i2cInterfaces;
};

#endif