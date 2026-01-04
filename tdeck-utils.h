#ifndef TDECK_UTIL_H
#define TDECK_UTIL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void TDeck_init(void);
void TDeck_printf(const char *fmt, ...);
void TDeck_printf_color(uint16_t color, const char *fmt, ...);

void TDeck_draw_circle(int x, int y, int r, uint16_t color);
void TDeck_draw_rectangle(int x, int y, int w, int h, uint16_t color, int filled);
void TDeck_draw_triangle(int x1,int y1,int x2,int y2,int x3,int y3,uint16_t color,int filled);

#ifdef __cplusplus
}
#endif

#endif
