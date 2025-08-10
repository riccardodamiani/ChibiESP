#include "core/kernel/drivers/wifi_driver.h"

#include <mutex>

#include <WiFi.h>
#include "esp_wifi.h"

/**
 * Enable the station mode (client mode) for Wi-Fi.
 */
bool WifiDriver::enableStation(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    wifi_mode_t currentMode = WiFi.getMode();
    if(currentMode == WIFI_OFF){
        WiFi.mode(WIFI_STA);
    }else if (currentMode == WIFI_AP) {
        WiFi.mode(WIFI_AP_STA);
    }
    return true;
}

/**
 * Disable the station mode (client mode) for Wi-Fi.
 */
bool WifiDriver::disableStation(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    wifi_mode_t currentMode = WiFi.getMode();
    if(currentMode == WIFI_STA){
        WiFi.mode(WIFI_OFF);
    }else if (currentMode == WIFI_AP_STA) {
        WiFi.mode(WIFI_AP);
    }
    return true;
}

/**
 * Check if the station mode (client mode) is enabled for Wi-Fi.
 * @return true if station mode is enabled, false otherwise.
 */
bool WifiDriver::isStationEnabled() const{
    wifi_mode_t currentMode = WiFi.getMode();
    return (currentMode == WIFI_STA || currentMode == WIFI_AP_STA);
}

/**
 * Connect to a Wi-Fi network.
 * @param ssid The SSID of the Wi-Fi network.
 * @param password The password for the Wi-Fi network.
 * @return An error code indicating the result of the operation.
 */
WifiErrorCode WifiDriver::connect(const char* ssid, const char* password){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    if(!isStationEnabled()){
        Logger::error("Cannot connect: Station mode is not enabled");
        return WifiErrorCode::MODE_DISABLED;
    }
    WiFi.begin(ssid, password);
    return WifiErrorCode::NONE;
}

/**
 * Disconnect from the currently connected Wi-Fi network.
 * @return An error code indicating the result of the operation.
 */
WifiErrorCode WifiDriver::disconnect(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    if(!isStationEnabled()){
        Logger::error("Cannot disconnect: Station mode is not enabled");
        return WifiErrorCode::MODE_DISABLED;
    }
    WiFi.disconnect();
    return WifiErrorCode::NONE;
}

/**
 * Check if the device is connected to a Wi-Fi network.
 * @return true if connected, false otherwise.
 */
bool WifiDriver::isConnected() const{
    return WiFi.isConnected();
}

/**
 * Get the local IP address of the device.
 * @return The local IP address.
 */
IPv4Address WifiDriver::getLocalIP() const{
    std::string localIP = WiFi.localIP().toString().c_str();
    return IPv4Address(localIP);
}

/**
 * Get the gateway IP address of the device.
 * @return The gateway IP address.
 */
IPv4Address WifiDriver::getGatewayIP() const{
    std::string gatewayIP = WiFi.gatewayIP().toString().c_str();
    return IPv4Address(gatewayIP);
}

/**
 * Get the subnet mask of the device.
 * @return The subnet mask.
 */
IPv4Address WifiDriver::getSubnetMask() const{
    std::string subnetMask = WiFi.subnetMask().toString().c_str();
    return IPv4Address(subnetMask);
}

/**
 * Get the RSSI (Received Signal Strength Indicator) of the connected Wi-Fi network.
 * @return The RSSI value in dBm.
 */
int32_t WifiDriver::getRSSI() const{
    return WiFi.RSSI();
}

/**
 * Get the SSID of the connected Wi-Fi network.
 * @return The SSID.
 */
std::string WifiDriver::getSSID() const{
    std::string ssid = WiFi.SSID().c_str();
    return ssid;
}

/**
 * Get the BSSID of the connected Wi-Fi network.
 * @return The BSSID.
 */
MacAddress WifiDriver::getBSSID() const{
    std::string bssid = WiFi.BSSIDstr().c_str();
    return MacAddress(bssid);
}

/**
 * Get the channel of the connected Wi-Fi network.
 * @return The channel.
 */
int WifiDriver::getChannel() const{
    return WiFi.channel();
}

/**
 * Set a static IP configuration for the device.
 * @param ip The static IP address.
 * @param gateway The gateway IP address.
 * @param subnet The subnet mask.
 * @return true if the configuration was successful, false otherwise.
 */
bool WifiDriver::setStaticIP(const IPv4Address& ip, const IPv4Address& gateway, const IPv4Address& subnet){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    WiFi.config(ip.toWiFi(), gateway.toWiFi(), subnet.toWiFi());
    return true;
}

/**
 * Scan for available Wi-Fi networks.
 * @param networks A vector to store the information of found networks.
 */
void WifiDriver::scanNetworks(std::vector<WifiNetworkInfo>& networks){
    std::lock_guard<std::mutex> lock(_wifiMutex);

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        WifiNetworkInfo info;
        info.ssid = std::string(WiFi.SSID(i).c_str());
        info.rssi = WiFi.RSSI(i);
        info.channel = WiFi.channel(i);
        info.encryptionType = WiFi.encryptionType(i);
        std::string mac = std::string(WiFi.BSSIDstr(i).c_str());
        info.bssid = MacAddress(mac);
        networks.push_back(info);
    }
    return;
}

/**
 * Enable Access Point mode.
 */
bool WifiDriver::enableAp(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    wifi_mode_t currentMode = WiFi.getMode();
    if(currentMode == WIFI_OFF){
        WiFi.mode(WIFI_AP);
    }else if (currentMode == WIFI_STA) {
        WiFi.mode(WIFI_AP_STA);
    }
    return true;
}

/**
 * Disable Access Point mode.
 */
bool WifiDriver::disableAp(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    wifi_mode_t currentMode = WiFi.getMode();
    if(currentMode == WIFI_AP_STA){
        WiFi.mode(WIFI_STA);
    }else if (currentMode == WIFI_AP) {
        WiFi.mode(WIFI_OFF);
    }
    return true;
}

bool WifiDriver::isApEnabled() const{
    wifi_mode_t currentMode = WiFi.getMode();
    return (currentMode == WIFI_AP || currentMode == WIFI_AP_STA);
}

/**
 * Set the network configuration for the Access Point.
 * @param localIP The local IP address.
 * @param gateway The gateway IP address.
 * @param subnet The subnet mask.
 * @return true if the configuration was successful, false otherwise.
 */
WifiErrorCode WifiDriver::setAPNetworkConfig(const IPv4Address& localIP, const IPv4Address& gateway, const IPv4Address& subnet){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    _apConfiguration.localIP = localIP;
    _apConfiguration.gatewayIP = gateway;
    _apConfiguration.subnetMask = subnet;

    if (!isApEnabled()) {
        Logger::error("Cannot set AP network config: Access Point mode is not enabled");
        return WifiErrorCode::MODE_DISABLED;
    }

    WiFi.softAPConfig(localIP.toWiFi(), gateway.toWiFi(), subnet.toWiFi());
    return WifiErrorCode::NONE;
}

/**
 * Create a new Access Point.
 * @param ssid The SSID of the Access Point.
 * @param password The password for the Access Point.
 * @param channel The channel for the Access Point (default is 1).
 * @param maxClients The maximum number of clients (default is 4).
 * @return true if the Access Point was created successfully, false otherwise.
 */
WifiErrorCode WifiDriver::createAP(const char* ssid, const char* password, int channel, int maxClients){
    std::lock_guard<std::mutex> lock(_wifiMutex);

    if (!isApEnabled()) {
        Logger::error("Cannot create AP: Access Point mode is not enabled");
        return WifiErrorCode::MODE_DISABLED;
    }

    WiFi.softAP(ssid, password, channel, false, maxClients);
    return WifiErrorCode::NONE;
}

/**
 * Stop the Access Point.
 * @return true if the Access Point was stopped successfully, false otherwise.
 */
bool WifiDriver::stopAP(){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    WiFi.softAPdisconnect();
    return true;
}

/**
 * Get the number of clients connected to the Access Point.
 * @return The number of connected clients.
 */
int WifiDriver::getAPClientCount() const{
    return WiFi.softAPgetStationNum();
}

/**
 * Get the list of clients connected to the Access Point.
 * @param clients A vector to store the information of connected clients.
 */
void WifiDriver::getAPConnectedClients(std::vector<ConnectedClient>& clients) {
    clients.clear();

    wifi_sta_list_t wifi_sta_list;
    tcpip_adapter_sta_list_t adapter_sta_list;

    if (esp_wifi_ap_get_sta_list(&wifi_sta_list) != ESP_OK) {
        return; // errore
    }

    if (tcpip_adapter_get_sta_list(&wifi_sta_list, &adapter_sta_list) != ESP_OK) {
        return; // errore
    }

    for (int i = 0; i < adapter_sta_list.num; i++) {
        tcpip_adapter_sta_info_t station = adapter_sta_list.sta[i];

        ConnectedClient client;
        client.ip = IPv4Address(station.ip.addr);
        client.mac = MacAddress(station.mac);

        clients.push_back(client);
    }
}

/**
 * Get the current network information.
 * @return The current network information.
 */
WifiNetworkInfo WifiDriver::getCurrentNetworkInfo() const{
    WifiNetworkInfo info;
    info.ssid = std::string(WiFi.SSID().c_str());
    info.rssi = WiFi.RSSI();
    info.channel = WiFi.channel();
    std::string bssid = std::string(WiFi.BSSIDstr().c_str());
    info.bssid = MacAddress(bssid);
    return info;
}

/**
 * Set the hostname for the Wi-Fi connection.
 * @param hostname The hostname to set.
 * @return true if the hostname was set successfully, false otherwise.
 */
bool WifiDriver::setHostname(const char* hostname){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    if (WiFi.setHostname(hostname)) {
        return true;
    } else {
        Logger::error("Failed to set hostname: %s", hostname);
        return false;
    }
}

/**
 * Set the transmission power for the Wi-Fi connection.
 * @param dbm The transmission power in dBm.
 * @return true if the transmission power was set successfully, false otherwise.
 */
bool WifiDriver::setTxPower(wifi_power_t dbm){
    std::lock_guard<std::mutex> lock(_wifiMutex);
    if (WiFi.setTxPower(dbm)) {
        return true;
    } else {
        Logger::error("Failed to set TX power: %d", dbm);
        return false;
    }
}
