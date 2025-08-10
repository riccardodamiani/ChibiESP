// Copyright (c) 2025 Riccardo Damiani
// Licensed under the Apache License, Version 2.0
// See LICENSE file in the project root for full license information.

#pragma once

#include <core/task/user_task.h>
#include <core/logging/logging.h>
#include <chibiESP.h>
#include <core/kernel/drivers/wifi_driver.h>
#include <core/kernel/drivers/ipv4.h>

#include <vector>
#include <ESPping.h>

const void wifi_program_setup(CESP_UserTaskData &taskData){
    Logger::info("Wifi program started");
    Logger::info("Enabling station mode..");
    chibiESP.wifi.enableStation();
    delay(1000);

    Logger::info("Scanning networks..");
    std::vector<WifiNetworkInfo> networks;
    chibiESP.wifi.scanNetworks(networks);
    for (const auto& network : networks) {
        Logger::info("## ssid: %s, MAC: %s", network.ssid.c_str(), network.bssid.toString().c_str());
    }

    delay(1000);
    Logger::info("Connecting to network..");
    chibiESP.wifi.connect("MY_SSID", "MY_PASSWORD");

    Logger::info("Waiting for connection...");
    while(!chibiESP.wifi.isConnected()) {
        delay(100);
    }
    
    Logger::info("Connected to WiFi!");
    Logger::info("Local IP: %s", chibiESP.wifi.getLocalIP().toString().c_str());
    Logger::info("Gateway IP: %s", chibiESP.wifi.getGatewayIP().toString().c_str());
    Logger::info("RSSI: %d", chibiESP.wifi.getRSSI());
    Logger::info("Channel: %d", chibiESP.wifi.getChannel());
}

const void wifi_program_loop(CESP_UserTaskData &taskData) {

    if (Ping.ping("google.com")) {
        Serial.println("google.com reachable!");
    } else {
        Serial.println("Ping failed.");
    }
    delay(2000);
}

const void wifi_program_closeup(CESP_UserTaskData &task){
    Logger::info("Closing wifi program");
    chibiESP.wifi.disconnect();
    chibiESP.wifi.disableStation();
}