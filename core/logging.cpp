#include "core/logging.h"

#include <cstdio>
#include <mutex>

std::mutex Logger::_mutex;

int Logger::init(){
    Serial.begin(115200);

    while(!Serial) delay(10); // wait for serial port to connect

    return 0;
}

void Logger::info(const char* fmt, ...) {

    char msgBuffer[192];
    char finalBuffer[256];

    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuffer, sizeof(msgBuffer), fmt, args);
    va_end(args);

    snprintf(finalBuffer, sizeof(finalBuffer), "Info: %s", msgBuffer);

    Serial.println(finalBuffer);
}

void Logger::warning(const char* fmt, ...){
    char msgBuffer[192];
    char finalBuffer[256];

    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuffer, sizeof(msgBuffer), fmt, args);
    va_end(args);

    snprintf(finalBuffer, sizeof(finalBuffer), "Warning: %s", msgBuffer);

    Serial.println(finalBuffer);
}

void Logger::error(const char* fmt, ...){
    char msgBuffer[192];
    char finalBuffer[256];

    std::lock_guard<std::mutex> lock(_mutex); // Lock the mutex for thread safety

    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuffer, sizeof(msgBuffer), fmt, args);
    va_end(args);

    snprintf(finalBuffer, sizeof(finalBuffer), "Error: %s", msgBuffer);

    Serial.println(finalBuffer);
}
