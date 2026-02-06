#ifndef SYMBOLS_H
uint32_t mbox_call(uint8_t chanel);
void serial_puts(const char* str);
void serial_putc(uint8_t c);
void serial_puti(uint32_t value);
void serial_putb(uint8_t value);
void clear_framebuffer();
void drawPixel(uint32_t x, uint32_t y, uint32_t color);
#define SYMBOLS_H
#endif
