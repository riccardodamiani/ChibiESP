#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <string>
#include <stdint.h>

class ChibiESP;

struct DisplayDriverInitStruct{
    ChibiESP* kernelInstance;
};

enum BW_Color{
    CESP_BLACK = 0,
    CESP_WHITE = 1
};

struct RGB_Color{
    uint8_t r, g, b;
};

class DisplayDriver{
public:
    DisplayDriver();
    virtual int configure(void* arg);
    virtual int init(DisplayDriverInitStruct *chibiESP_Instance);
    virtual int deinit(void* arg);
    virtual int updateScreen();
    virtual int clearScreen();

    //functions for monochromatic displays
    virtual int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, BW_Color color);
    virtual int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, BW_Color color);
    virtual int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, BW_Color color);
    virtual int drawText(std::string text, int16_t x, int16_t y, int16_t size, BW_Color bg_color, BW_Color fg_color);
    virtual int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, BW_Color bg_color, BW_Color fg_color);
    virtual int fillScreen(BW_Color color);

    //functions for colored displays
    virtual int drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, RGB_Color color);
    virtual int drawRect(int16_t x, int16_t y, int16_t width, int16_t height, bool fill, RGB_Color color);
    virtual int drawCircle(int16_t x, int16_t y, int16_t radius, bool fill, RGB_Color color);
    virtual int drawText(std::string text, int16_t x, int16_t y, int16_t size, RGB_Color bg_color, RGB_Color fg_color);
    virtual int drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const int16_t width, const int16_t height, RGB_Color bg_color, RGB_Color fg_color);
    virtual int fillScreen(RGB_Color color);

    //info functions
    virtual int getTextSize(std::string text, int16_t x, int16_t y, int16_t size, int16_t *real_x, int16_t *real_y, uint16_t* width, uint16_t* height);
    virtual int get_device_info(void* arg);
    std::string get_name() const { return _name; }
protected:
    std::string _name; // Name of the driver
};

#endif  //DISPLAY_DRIVER_H