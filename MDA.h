#ifndef MDA_H
#define MDA_H
#include <stdint.h>
#include <stddef.h>
void MDA_init();
void MDA_draw_char(uint8_t c, uint32_t x, uint32_t y, uint32_t color, uint32_t bgcolor);
void MDA_show_string(const char* str, uint32_t x, uint32_t y, uint32_t color, uint32_t bgcolor);
void MDA_update();
#endif
