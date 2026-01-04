        #include "tdeck-utils.h"

        void setup() {
            TDeck_init();
            TDeck_set_volume(100);

            int size = 80;
            int radius = size/2 - 5;
            int tri_h = size/2;

            int sx = TFT_WIDTH - size;
            int sy = 0;
            int cx = sx + size/2;
            int cy = sy + size + size/2;
            int tx = cx;
            int ty = cy + size;

            TDeck_draw_rectangle(sx, sy, size, size, TFT_BLUE, 1);
            TDeck_draw_circle(cx, cy, radius, TFT_GREEN, 1);
            TDeck_draw_triangle(tx, ty - tri_h/2, tx - tri_h/2, ty + tri_h/2, tx + tri_h/2, ty + tri_h/2, TFT_RED, 1);

            int csx = (TFT_WIDTH - size) / 2;
            int csy = (TFT_HEIGHT - size*3) / 2;
            int ccx = csx + size/2;
            int ccy = csy + size + size/2;
            int ctx = ccx;
            int cty = ccy + size;

            TDeck_draw_rectangle(csx, csy, size, size, TFT_BLUE, 0);
            TDeck_draw_circle(ccx, ccy, radius, TFT_GREEN, 0);
            TDeck_draw_triangle(ctx, cty - tri_h/2, ctx - tri_h/2, cty + tri_h/2, ctx + tri_h/2, cty + tri_h/2, TFT_RED, 0);

            TDeck_font_size(1);
            TDeck_printf_color(TFT_RED, "RED\n");
            TDeck_font_size(2);
            TDeck_printf_color(TFT_GREEN, "GREEN\n");
            TDeck_font_size(3);
            TDeck_printf_color(TFT_BLUE, "BLUE\n");
            TDeck_font_size(1);

            TDeck_play_mp3("music.mp3");
        }

        void loop() {
            char key = TDeck_getchar();
            if (key != 0) TDeck_printf("%c", key);
            TDeck_audio_loop();
        }
