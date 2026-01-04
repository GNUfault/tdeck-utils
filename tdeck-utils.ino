#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"
#include "tdeck-utils.h"

TFT_eSPI tft = TFT_eSPI();

int cx = 0;
int cy = 0;
int lh = 0;
int cw = 0;
uint16_t col = TFT_WHITE;

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
    pinMode(BOARD_SDCARD_CS, OUTPUT);
    pinMode(RADIO_CS_PIN, OUTPUT);
    pinMode(BOARD_TFT_CS, OUTPUT);
    digitalWrite(BOARD_SDCARD_CS, HIGH);
    digitalWrite(RADIO_CS_PIN, HIGH);
    digitalWrite(BOARD_TFT_CS, HIGH);
    pinMode(BOARD_SPI_MISO, INPUT_PULLUP);
    SPI.begin(BOARD_SPI_SCK, BOARD_SPI_MISO, BOARD_SPI_MOSI);
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

void TDeck_draw_circle(int x, int y, int r, uint16_t color) {
    tft.drawCircle(x, y, r, color);
}

void TDeck_draw_rectangle(int x, int y, int w, int h, uint16_t color) {
    tft.drawRect(x, y, w, h, color);
}

void TDeck_draw_triangle(int x1,int y1,int x2,int y2,int x3,int y3,uint16_t color) {
    tft.drawTriangle(x1,y1,x2,y2,x3,y3,color);
}
