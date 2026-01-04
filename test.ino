#include "tdeck-utils.h"

void setup() {
    int sq = 80;

    int sx = TFT_WIDTH - sq;
    int sy = 0;

    int cx = sx + sq/2;
    int cy = sy + sq/2;
    
    TDeck_init();

    TDeck_printf_color(TFT_RED, "RED\n");
    TDeck_printf_color(TFT_GREEN, "GREEN\n");
    TDeck_printf_color(TFT_BLUE, "BLUE\n");
    TDeck_printf("NORMAL\n");

    TDeck_draw_rectangle(sx, sy, sq, sq, TFT_BLUE);
    TDeck_draw_circle(cx, cy, 35, TFT_GREEN);
    TDeck_draw_triangle(cx, cy-25, cx-22, cy+15, cx+22, cy+15, TFT_RED);

}

void loop() {
    char key = TDeck_getchar();
    if (key != 0) {
        TDeck_printf("%c", key);
    }
    delay(5);
}
