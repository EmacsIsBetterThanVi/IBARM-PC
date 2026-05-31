#ifndef SYMBOLS_H
#include <stdint.h>
uint32_t mbox_call(uint8_t);
void serial_puts(const char*);
void serial_putc(uint8_t);
void serial_puti(uint32_t);
void serial_putb(uint8_t);
void fill_framebuffer(uint32_t);
#define clear_framebuffer() fill_framebuffer(0)
void drawPixel(uint32_t, uint32_t, uint32_t);
struct FNS { // Fns is a struct of functions defined for the sole purpose of
  //           consistancy in driver programing.
  void (*VideoUpdate)();
  uint8_t (*getChar)();
};
extern struct FNS Fns;
extern uint8_t *videoram;
extern uint32_t videoramlen;
#define SYMBOLS_H
#endif
