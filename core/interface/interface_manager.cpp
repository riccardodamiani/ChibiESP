#include "core/interface/interface_manager.h"
#include "core/interface/i2c_bus.h"
#include "core/logging/logging.h"

#include <map>

class TwoWire;

bool InterfaceManager::registerI2cInterface(int bus, int sda_pin, int scl_pin){
    if(bus > 255 || bus < 0){
        return false;
    }

    //already exists
    if(_i2cInterfaces.find(bus) != _i2cInterfaces.end()){
        return false;
    }    
    I2cBus* newBus = new I2cBus(bus, sda_pin, scl_pin);
    if(!newBus || !newBus->init()){
        if(newBus) delete newBus;
        Logger::error("InterfaceManager: error creating I2c interface on bus %d", bus);
        return false;
    }
    _i2cInterfaces[bus] = newBus;
    Logger::info("InterfaceManager: Creating I2c interface on bus %d", bus);
    return true;
}

TwoWire* InterfaceManager::getI2cInterface(int bus){
    if(_i2cInterfaces.find(bus) == _i2cInterfaces.end()){
        return nullptr;
    }
    return _i2cInterfaces[bus]->getWire();
}