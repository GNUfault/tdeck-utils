#pragma once
#include <Arduino.h>

#define TFT_WIDTH 320
#define TFT_HEIGHT 240

void TDeck_init();
void TDeck_printf(const char *fmt, ...);
void TDeck_printf_color(uint16_t color, const char *fmt, ...);
void TDeck_draw_circle(int x, int y, int r, uint16_t color);
void TDeck_draw_rectangle(int x, int y, int w, int h, uint16_t color);
void TDeck_draw_triangle(int x1,int y1,int x2,int y2,int x3,int y3,uint16_t color);
char TDeck_getchar();
void TDeck_keyboard_backlight(uint8_t value);
void TDeck_font(uint8_t font);
void TDeck_font_size(uint8_t size);
void TDeck_set_volume(uint8_t volume);
bool TDeck_play_mp3(const char* filename);
void TDeck_stop_mp3();
bool TDeck_is_playing();
void TDeck_audio_loop();
