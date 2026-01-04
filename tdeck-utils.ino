#include "tdeck-utils.h"
#include "utilities.h"
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static TFT_eSPI tft = TFT_eSPI();

static int cursor_x = 0;
static int cursor_y = 0;
static int line_height = 0;
static int char_width = 0;

static const int SCREEN_WIDTH = 320;
static const int SCREEN_HEIGHT = 240;

static void update_metrics(void) {
    line_height = tft.fontHeight() + 2;
    char_width  = tft.textWidth("W");
}

void Display_init(void) {
    pinMode(BOARD_POWERON, OUTPUT);
    digitalWrite(BOARD_POWERON, HIGH);

    pinMode(BOARD_SDCARD_CS, OUTPUT);
    pinMode(RADIO_CS_PIN, OUTPUT);
    pinMode(BOARD_TFT_CS, OUTPUT);

    digitalWrite(BOARD_SDCARD_CS, HIGH);
    digitalWrite(RADIO_CS_PIN, HIGH);
    digitalWrite(BOARD_TFT_CS, HIGH);

    pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);

    tft.begin();
    
    pinMode(BOARD_BL_PIN, OUTPUT);
    digitalWrite(BOARD_BL_PIN, HIGH);
    
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    tft.setTextDatum(TL_DATUM);
    tft.setTextFont(2);

    update_metrics();

    cursor_x = 0;
    cursor_y = 0;
}

static void put_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y += line_height;

        if (cursor_y + line_height > SCREEN_HEIGHT) {
            cursor_y = 0;
            tft.fillScreen(TFT_BLACK);
        }
        return;
    }

    if (c == '\r') {
        cursor_x = 0;
        return;
    }

    if (cursor_x + char_width > SCREEN_WIDTH) {
        cursor_x = 0;
        cursor_y += line_height;

        if (cursor_y + line_height > SCREEN_HEIGHT) {
            cursor_y = 0;
            tft.fillScreen(TFT_BLACK);
        }
    }

    char buf[2] = {c, 0};
    tft.drawString(buf, cursor_x, cursor_y);
    cursor_x += char_width;
}

void Display_printf(const char *fmt, ...) {
    char buffer[512];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    for (int i = 0; buffer[i]; i++) {
        put_char(buffer[i]);
    }
}
