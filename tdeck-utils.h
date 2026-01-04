#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

void Display_init(void);
void Display_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
