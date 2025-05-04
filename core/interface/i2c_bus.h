#ifndef I2C_BUS_H
#define I2C_BUS_H

#include <Wire.h>

class I2cBus {
public:
    I2cBus(int bus, int sda_pin, int scl_pin)
        : _bus(bus), _sda(sda_pin), _scl(scl_pin), _wire(nullptr) {}

    ~I2cBus() {
        if (_wire) delete _wire;
    }

    bool init() {
        _wire = new TwoWire(_bus);
        if(!_wire) return false;
        _wire->begin(_sda, _scl);
        return true;
    }

    TwoWire* getWire() const {
        return _wire;
    }

    int getBus() const {
        return _bus;
    }

private:
    int _bus;
    int _sda, _scl;
    TwoWire* _wire;
};

#endif //I2C_BUS_H