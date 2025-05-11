/**
 * @file bw_display_driver.cpp
 * @brief Display driver template
 * @details Template for a generic display driver. overload this functions to a driver for a specific display
 */

#include "core/kernel/driver/display_driver.h"

DisplayDriver::DisplayDriver() : 
    _name("PlaceholderDisplayDriver")
{

}


/**
 * @brief Configuration function. 
 * @details This configure the driver for the specific hardware (gpio, ecc..). This function is called by the user before registering the driver in the kernel
 * @param args is a pointer to a user defined structure. It's dependent on the hardware
 */
int DisplayDriver::configure(void* arg){
    return 0;
}

/**
 * @brief TODO: Function to get device information
 */
int DisplayDriver::get_device_info(int deviceId, DisplayDeviceInfo_t &info){
    return 0;
}

/**
 * @brief TODO: Function to get the size in pixels the text will have on screen
 */
int DisplayDriver::getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height){
    return 0;
}


/**
 * @brief Driver initialization function
 * @details This is called by the kernel when the driver is loaded (and starts running). This should contain hardware inizialization stuff
 */
int DisplayDriver::init(){
    return 0;
}

/**
 * @brief Driver de-initialization function
 * @details This is called by the kernel when the driver is unloaded (stops running). This should contain hardware de-initialization stuff, memory cleaning, ecc..
 */
int DisplayDriver::deinit(void* arg){
    return 0;
}

/**
 * @brief Driver update screen function function
 * @details Draws the internal screen buffer on screen
 */
int DisplayDriver::updateScreen(){
    return 0;
}

/**
 * @brief Clear the internal display buffer
 */
int DisplayDriver::clearScreen(){
    return 0;
}

/*********************************
* Monochromatic display functions
/************************************/

/**
 * @brief Function to draw a line on screen
 */
int DisplayDriver::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a rectangle on screen
 */
int DisplayDriver::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a circle on screen
 */
int DisplayDriver::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color){
    return 0;
}

/**
 * @brief Function to draw a text on screen
 */
int DisplayDriver::drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color){
    return 0;
}

/**
 * @brief Function to draw a bitmap (1 bit per color) on screen
 */
int DisplayDriver::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color){
    return 0;
}

/**
 * @brief Function to fill the screen with one color
 */
int DisplayDriver::fillScreen(BW_Color color){
    return 0;
}


/*********************************
* RGB display functions
/************************************/

/**
 * @brief Function to draw a line on screen
 */
int DisplayDriver::drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a rectangle on screen
 */
int DisplayDriver::drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a circle on screen
 */
int DisplayDriver::drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color){
    return 0;
}
/**
 * @brief Function to draw a text on screen
 */
int DisplayDriver::drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color){
    return 0;
}
/**
 * @brief Function to draw a bitmap (1 bit per color) on screen
 */
int DisplayDriver::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color){
    return 0;
}
/**
 * @brief Function to fill the screen with one color
 */
int DisplayDriver::fillScreen(RGB_Color color){
    return 0;
}
