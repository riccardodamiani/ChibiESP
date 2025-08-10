#ifndef WIFI_DRIVER_H
#define WIFI_DRIVER_H

#include <vector>
#include <array>
#include <functional>
#include <string>
#include <mutex>

#include "core/kernel/drivers/ipv4.h"

enum class WifiErrorCode{
    NONE,
    MODE_DISABLED,
    NOT_CONNECTED
};

struct ConnectedClient {
    MacAddress mac;
    IPv4Address ip;
};

struct WifiNetworkInfo {
    std::string ssid;
    int32_t rssi;
    int channel;
    MacAddress bssid;
    uint8_t encryptionType;
};

class WifiDriver {
public:
    struct NetworkConfig {
        IPv4Address localIP;
        IPv4Address gatewayIP;
        IPv4Address subnetMask;
    };
    
    WifiDriver() = default;
    ~WifiDriver() = default;

    // Elimina operatori di copia e move
    WifiDriver(const WifiDriver&) = delete;
    WifiDriver& operator=(const WifiDriver&) = delete;
    WifiDriver(WifiDriver&&) = delete;
    WifiDriver& operator=(WifiDriver&&) = delete;

    // --- Station mode ---
    bool enableStation();
    bool disableStation();
    bool isStationEnabled() const;
    WifiErrorCode connect(const char* ssid, const char* password);
    WifiErrorCode disconnect();
    bool isConnected() const;
    IPv4Address getLocalIP() const;
    IPv4Address getGatewayIP() const;
    IPv4Address getSubnetMask() const;
    int32_t getRSSI() const;
    std::string getSSID() const;
    MacAddress getBSSID() const;
    int getChannel() const;
    bool setStaticIP(const IPv4Address& ip, const IPv4Address& gateway, const IPv4Address& subnet);
    void scanNetworks(std::vector<WifiNetworkInfo>& networks);

    // --- Access Point mode ---
    bool enableAp();
    bool disableAp();
    bool isApEnabled() const;
    WifiErrorCode setAPNetworkConfig(const IPv4Address& localIP, const IPv4Address& gateway, const IPv4Address& subnet);
    WifiErrorCode createAP(const char* ssid, const char* password, int channel = 1, int maxClients = 4);
    bool stopAP();
    int getAPClientCount() const;
    void getAPConnectedClients(std::vector<ConnectedClient>& clients);

    // TODO: create blacklist functions
    // --- Blacklist MAC ---
    //bool addMACToBlacklist(const MacAddress& mac);
    //bool removeMACFromBlacklist(const MacAddress& mac);
    //std::vector<MacAddress> getBlacklistedMACs() const;

    // --- Utility ---
    WifiNetworkInfo getCurrentNetworkInfo() const;
    bool setHostname(const char* hostname);
    bool setTxPower(wifi_power_t dbm);

private:
    NetworkConfig _apConfiguration;

    std::mutex _wifiMutex;
};

#endif  // WIFI_DRIVER_H