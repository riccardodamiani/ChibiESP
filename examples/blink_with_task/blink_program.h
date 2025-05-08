#pragma once

#include <core/task/user_task.h>
#include <core/logging/logging.h>
#include <chibiESP.h>
#include <Adafruit_NeoPixel.h>
#define LED_PIN    48
#define NUM_LEDS   1

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

const void blink_program_setup(CESP_UserTaskData &taskData){
    Logger::info("Blink program started");
    strip.begin();
    strip.show(); // Spegne tutto
}

const void blink_program_loop(CESP_UserTaskData &taskData) {
    static uint32_t lastUpdate = 0;
    static uint8_t currentColor = 0;

    uint32_t now = millis();
    if (now - lastUpdate >= 1000) {
        lastUpdate = now;

        switch (currentColor) {
            case 0:
                strip.setPixelColor(0, strip.Color(20, 0, 0)); // Rosso
                break;
            case 1:
                strip.setPixelColor(0, strip.Color(0, 20, 0)); // Verde
                break;
            case 2:
                strip.setPixelColor(0, strip.Color(0, 0, 25)); // Blu
                break;
        }

        strip.show();
        currentColor = (currentColor + 1) % 3;
    }
    delay(100);
}

const void blink_program_closeup(CESP_UserTaskData &task){
    Logger::info("Closing button program");
    strip.clear(); // Spegne
    strip.show();
}