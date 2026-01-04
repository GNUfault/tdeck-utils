#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <SD.h>
#include <FS.h>
#include "Audio.h"
#include "utilities.h"
#include "tdeck-utils.h"

TFT_eSPI tft = TFT_eSPI();
Audio audio;

int cx = 0;
int cy = 0;
int lh = 0;
int cw = 0;
uint16_t col = TFT_WHITE;
uint8_t current_volume = 50;

void upd() {
    lh = tft.fontHeight();
    cw = tft.textWidth("W");
}

void scr() {
    int h = TFT_HEIGHT - lh;
    int n = TFT_WIDTH * h;
    uint16_t *buf = (uint16_t*) malloc(n * 2);
    if (!buf) return;
    tft.readRect(0, lh, TFT_WIDTH, h, buf);
    tft.pushImage(0, 0, TFT_WIDTH, h, buf);
    free(buf);
    tft.fillRect(0, h, TFT_WIDTH, lh, TFT_BLACK);
}

void TDeck_init() {
    pinMode(BOARD_POWERON, OUTPUT);
    digitalWrite(BOARD_POWERON, HIGH);

    delay(500);

    pinMode(RADIO_CS_PIN, OUTPUT);
    pinMode(BOARD_SDCARD_CS, OUTPUT);
    pinMode(BOARD_TFT_CS, OUTPUT);
    digitalWrite(BOARD_SDCARD_CS, HIGH);
    digitalWrite(RADIO_CS_PIN, HIGH);
    digitalWrite(BOARD_TFT_CS, HIGH);
    pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);

    SD.begin(BOARD_SDCARD_CS);

    audio.setPinout(BOARD_I2S_BCK, BOARD_I2S_WS, BOARD_I2S_DOUT);
    audio.setVolume(10); 

    TDeck_set_volume(current_volume);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextFont(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    upd();
    cx = 0;
    cy = 0;
    pinMode(BOARD_BL_PIN, OUTPUT);
    digitalWrite(BOARD_BL_PIN, HIGH);

    Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);

    Wire.requestFrom(LILYGO_KB_SLAVE_ADDRESS, 1);
    if (Wire.read() == -1) {
        TDeck_printf("Keyboard not found\n");
    }
}

void putc2(char c) {
    if (c == '\n') {
        cx = 0;
        cy += lh;
        if (cy + lh > TFT_HEIGHT) {
            scr();
            cy = TFT_HEIGHT - lh;
        }
        return;
    }
    if (c == '\r') {
        cx = 0;
        return;
    }
    if (cx + cw > TFT_WIDTH) {
        cx = 0;
        cy += lh;
        if (cy + lh > TFT_HEIGHT) {
            scr();
            cy = TFT_HEIGHT - lh;
        }
    }
    char b[2] = {c,0};
    tft.setTextColor(col, TFT_BLACK);
    tft.drawString(b, cx, cy);
    cx += cw;
}

void TDeck_printf(const char *fmt, ...) {
    col = TFT_WHITE;
    char buf[512];
    va_list a;
    va_start(a, fmt);
    vsnprintf(buf, sizeof(buf), fmt, a);
    va_end(a);
    for (int i = 0; buf[i]; i++) putc2(buf[i]);
}

void TDeck_printf_color(uint16_t color, const char *fmt, ...) {
    col = color;
    char buf[512];
    va_list a;
    va_start(a, fmt);
    vsnprintf(buf, sizeof(buf), fmt, a);
    va_end(a);
    for (int i = 0; buf[i]; i++) putc2(buf[i]);
}

void TDeck_draw_circle(int x, int y, int r, uint16_t color, int filled) {
    if (filled) {
        tft.fillCircle(x, y, r, color);
    } else {
        tft.drawCircle(x, y, r, color);
    }
}

void TDeck_draw_rectangle(int x, int y, int w, int h, uint16_t color, int filled) {
    if (filled) {
        tft.fillRect(x, y, w, h, color);
    } else {
        tft.drawRect(x, y, w, h, color);
    }
}

void TDeck_draw_triangle(int x1,int y1,int x2,int y2,int x3,int y3,uint16_t color, int filled) {
    if (filled) {
        tft.fillTriangle(x1, y1, x2, y2, x3, y3, color);
    } else {
        tft.drawTriangle(x1, y1, x2, y2, x3, y3, color);
    }
}

char TDeck_getchar() {
    char keyValue = 0;
    Wire.requestFrom(LILYGO_KB_SLAVE_ADDRESS, 1);
    while (Wire.available() > 0) {
        keyValue = Wire.read();
        if (keyValue != (char)0x00) {
            return keyValue;
        }
    }
    return 0;
}

void TDeck_keyboard_backlight(uint8_t value) {
    Wire.beginTransmission(LILYGO_KB_SLAVE_ADDRESS);
    Wire.write(LILYGO_KB_BRIGHTNESS_CMD);
    Wire.write(value);
    Wire.endTransmission();
}

void TDeck_font(uint8_t font) {
    tft.setTextFont(font);
    upd();
}

void TDeck_font_size(uint8_t size) {
    tft.setTextSize(size);
    upd();
}

void TDeck_set_volume(uint8_t volume) {
    if (volume > 100) volume = 100;
    current_volume = volume;

    uint8_t audioVolume = (volume * 21) / 100;
    audio.setVolume(audioVolume);
}

bool TDeck_play_mp3(const char* filename) {
    String path = "/";
    path += filename;

    if (!SD.exists(path)) {
        TDeck_printf("File not found: %s\n", filename);
        return false;
    }

    bool success = audio.connecttoSD(path.c_str());
    return success;
}

void TDeck_stop_mp3() {
    audio.stopSong();
}

bool TDeck_is_playing() {
    return audio.isRunning();
}

void TDeck_audio_loop() {
    audio.loop();
}
