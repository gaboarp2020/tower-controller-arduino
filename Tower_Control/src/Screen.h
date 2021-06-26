#ifndef Screen_h
#define Screen_h

#include <SPI.h>
#include <TFT_eSPI.h>

#include "Serial.h"

#ifndef SCREEN_ORIENTATION
#define SCREEN_ORIENTATION 1
#endif

#ifndef SCREEN_FONT
#define SCREEN_FONT 1
#endif

#ifndef SCREEN_SIZE
#define SCREEN_SIZE 1
#endif

// The TFT should be defined outside of the Screen class to avoid weird behaviors
TFT_eSPI _tft = TFT_eSPI();

class Screen
{
public:
    Screen();
    void begin();
    void readDisplaySetup();
    void text(String message, int16_t x, int16_t y, uint16_t fgcolor, uint16_t bgcolor, uint8_t size, uint8_t font);
    // TFT Methods
    uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);
    void drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
    void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
    virtual void drawPixel(int32_t x, int32_t y, uint32_t color);
    void fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
    void fillScreen(uint32_t color);
    int16_t height();
    void invertDisplay(bool i);
    void print(int line);
    void print(float line);
    void print(String line);
    void println(int line);
    void println(float line);
    void println(String line);
    void setCursor(int16_t x, int16_t y);
    void setCursor(int16_t x, int16_t y, uint8_t font);
    void setTextColor(uint16_t color);
    void setTextColor(uint16_t fgcolor, uint16_t bgcolor);
    void setTextDatum(uint8_t d);
    void setTextFont(uint8_t font);
    void setTextSize(uint8_t size);
    int16_t width();

private:
};

Screen::Screen()
{
}

void Screen::begin()
{
    _tft.init();
    _tft.setRotation(SCREEN_ORIENTATION);
    _tft.fillScreen(TFT_BLACK);
}

void Screen::readDisplaySetup()
{
    setup_t config;
    _tft.getSetup(config);

    consoleLog("\n---------------------------------------------------------\n");
    consoleLog("\t\t DISPLAY CONFIGURATION\n");

    // For ePaper displays the size is defined in the sketch
    if (config.tft_driver != 0xE9D)
    {
        consoleLog("\tDisplay driver = " + String(config.tft_driver));
        consoleLog("\tDisplay width  = " + String(config.tft_width));
        consoleLog("\tDisplay height = " + String(config.tft_height));
    }
    else if (config.tft_driver == 0xE9D)
        consoleLog("\tDisplay driver = ePaper");

    if (config.serial == 1)
    {
        consoleLog("\tDisplay SPI frequency = " + String(config.tft_spi_freq / 10.0));
    }

    // Offsets, not all used yet
    if (config.r0_x_offset != 0)
    {
        consoleLog("\tR0 x offset = " + String(config.r0_x_offset));
    }
    if (config.r0_y_offset != 0)
    {
        consoleLog("\tR0 y offset = " + String(config.r0_y_offset));
    }
    if (config.r1_x_offset != 0)
    {
        consoleLog("\tR1 x offset = " + String(config.r1_x_offset));
    }
    if (config.r1_y_offset != 0)
    {
        consoleLog("\tR1 y offset = " + String(config.r1_y_offset));
    }
    if (config.r2_x_offset != 0)
    {
        consoleLog("\tR2 x offset = " + String(config.r2_x_offset));
    }
    if (config.r2_y_offset != 0)
    {
        consoleLog("\tR2 y offset = " + String(config.r2_y_offset));
    }
    if (config.r3_x_offset != 0)
    {
        consoleLog("\tR3 x offset = " + String(config.r3_x_offset));
    }
    if (config.r3_y_offset != 0)
    {
        consoleLog("\tR3 y offset = " + String(config.r3_y_offset));
    }

    consoleLog("\n---------------------------------------------------------\n");
}

void Screen::text(String message, int16_t x, int16_t y, uint16_t fgcolor, uint16_t bgcolor, uint8_t size = SCREEN_SIZE, uint8_t font = SCREEN_FONT)
{
    _tft.setCursor(x, y, font);
    _tft.setTextSize(size);
    _tft.setTextColor(fgcolor, bgcolor);
    _tft.print(message);
}

// TFT Methods

uint16_t Screen::color565(uint8_t red, uint8_t green, uint8_t blue)
{
    _tft.color565(red, green, blue);
}

void Screen::drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color)
{
    _tft.drawCircle(x, y, r, color);
}

void Screen::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
{
    _tft.drawFastHLine(x, y, w, color);
}

void Screen::drawPixel(int32_t x, int32_t y, uint32_t color)
{
    _tft.drawPixel(x, y, color);
}

void Screen::fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color)
{
    _tft.fillCircle(x, y, r, color);
}

void Screen::fillScreen(uint32_t color)
{
    _tft.fillScreen(color);
}

int16_t Screen::height()
{
    return _tft.height();
}

void Screen::invertDisplay(bool i)
{
    _tft.invertDisplay(i);
}

void Screen::print(int line)
{
    _tft.print(line);
}

void Screen::print(float line)
{
    _tft.print(line);
}

void Screen::print(String line)
{
    _tft.print(line);
}

void Screen::println(int line)
{
    _tft.println(line);
}

void Screen::println(float line)
{
    _tft.println(line);
}

void Screen::println(String line)
{
    _tft.println(line);
}

void Screen::setCursor(int16_t x, int16_t y)
{
    this->setCursor(x, y, 1);
}

void Screen::setCursor(int16_t x, int16_t y, uint8_t font)
{
    _tft.setCursor(x, y, font);
}

void Screen::setTextColor(uint16_t color)
{
    _tft.setTextColor(color);
}

void Screen::setTextColor(uint16_t fgcolor, uint16_t bgcolor)
{
    _tft.setTextColor(fgcolor, bgcolor);
}

void Screen::setTextDatum(uint8_t d)
{
    _tft.setTextDatum(d);
}

void Screen::setTextFont(uint8_t font)
{
    _tft.setTextFont(font);
}

void Screen::setTextSize(uint8_t size)
{
    _tft.setTextSize(size);
}

int16_t Screen::width()
{
    return _tft.width();
}

#endif
