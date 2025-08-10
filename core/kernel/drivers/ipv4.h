#ifndef IPV4_H
#define IPV4_H

#include "core/logging/logging.h"

#include <array>
#include <string>
#include <sstream>

#include <WiFi.h>

namespace {
    inline bool strToUint8(const std::string& str, uint8_t& out, int base = 10) {
        char* endptr = nullptr;
        long value = strtol(str.c_str(), &endptr, base);
        if (*endptr == '\0' && value >= 0 && value <= 255) {
            out = static_cast<uint8_t>(value);
            return true;
        }
        return false; // Conversion failed
    }
};

class MacAddress {
public:
    MacAddress() : addr{} {}
    MacAddress(const std::string& address) {
        parseMacAddress(address, addr);
    }
    MacAddress(const std::array<uint8_t, 6>& address) {
        addr = address;
    }
    MacAddress(const uint8_t mac[6]) {
        for (int i = 0; i < 6; ++i) {
            addr[i] = mac[i];
        }
    }

    // convert to string
    std::string toString() const {
        char buffer[18]; // max "FF:FF:FF:FF:FF:FF\0"
        snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
        return buffer;
    }

    // set the mac address from a string
    bool setAddress(const std::string& address) {
        return parseMacAddress(address, addr);
    }
private:
    std::array<uint8_t, 6> addr;

    // parse a mac string into the mac address
    inline bool parseMacAddress(const std::string& macStr, std::array<uint8_t, 6>& out) {
        std::istringstream ss(macStr);
        std::string token;
        int i = 0;
        while (std::getline(ss, token, ':') && i < 6) {
            uint8_t value;
            if (!strToUint8(token, value, 16)) {
                return false; // Conversion failed
            }
            out[i++] = value;
        }
        return i == 6;
    }
};

struct IPv4Address {
public:
    IPv4Address() : addr{} {}

    IPv4Address(const std::string& address) {
        parseIPv4Address(address, addr);
    }
    IPv4Address(const std::array<uint8_t, 4>& address) {
        addr = address;
    }
    IPv4Address(uint32_t address) {
        addr[0] = (address >> 24) & 0xFF;
        addr[1] = (address >> 16) & 0xFF;
        addr[2] = (address >> 8) & 0xFF;
        addr[3] = address & 0xFF;
    }

    // convert to string
    std::string toString() const {
        char buffer[16]; // max "255.255.255.255\0"
        snprintf(buffer, sizeof(buffer), "%u.%u.%u.%u",
                addr[0], addr[1], addr[2], addr[3]);
        return buffer;
    }

    // set the ipv4 address from a string
    bool setAddress(const std::string& address) {
        return parseIPv4Address(address, addr);
    }

    IPAddress toWiFi() const {
        return IPAddress(addr[0], addr[1], addr[2], addr[3]);
    }

private:
    std::array<uint8_t, 4> addr;

    // parse a ipv4 string into the ipv4 address
    static bool parseIPv4Address(const std::string& ipStr, std::array<uint8_t, 4>& out) {
        std::istringstream ss(ipStr);
        std::string token;
        int i = 0;
        while (std::getline(ss, token, '.') && i < 4) {
            uint8_t value;
            if (!strToUint8(token, value, 10)) {
                return false; // Conversion failed
            }
            out[i++] = value;
        }
        return i == 4;
    }
};

#endif  // IPV4_H