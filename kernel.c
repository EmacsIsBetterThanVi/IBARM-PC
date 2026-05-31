#include <stddef.h>
#include <stdint.h>
#include "symbols.h"
#include "GPIO.h"
#ifndef DEBUG
#define DEBUG 0
#endif
static inline void delay(int32_t count){
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}
volatile uint32_t  __attribute__((aligned(16))) mbox[36] = {
    36, 0, 0x38002, 12, 8, 2, 3000000, 0 ,0
};
uint32_t fb_width, fb_height, fb_pitch, fb_rgbp;
uint8_t *fb;
uint32_t mbox_call(uint8_t chanel){
		 uint32_t r = (((uint32_t)(&mbox) &~ 0xF) | (chanel & 0xF));
		 while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
		 mmio_write(MBOX_WRITE, r);
		 while (1){
			 while (mmio_read(MBOX_STATUS) & 0x40000000);
			 if (r == mmio_read(MBOX_READ)) return mbox[1]==0x80000000;
		}
		return 0;
	}
void frame_buffer_init(uint32_t width, uint32_t height){
	mbox[0] = 140;
	mbox[1] = 0x80000000;
	// SET PHYSICAL SIZE
	mbox[2] = 0x00048003;
	mbox[3] = 8;
	mbox[4] = 0;
	mbox[5] = width;
	mbox[6] = height;
	// SET VIRTUAL SIZE
	mbox[7] = 0x00048004;
	mbox[8] = 8;
	mbox[9] = 8;
	mbox[10] = width;
	mbox[11] = height;
	// SET VIRTUAL OFFSET
	mbox[12] = 0x00048009;
	mbox[13] = 8;
	mbox[14] = 8;
	mbox[15] = 0;
	mbox[16] = 0;
	// SET DEPTH
	mbox[17] = 0x00048005;
	mbox[18] = 4;
	mbox[19] = 4;
	mbox[20] = 32;
	// SET AS RBG
	mbox[21] = 0x00048006;
	mbox[22] = 4;
	mbox[23] = 4;
	mbox[24] = 1;
	// GET FRAME BUFFER
	mbox[25] = 0x00040001;
	mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096;
    mbox[29] = 0;
    // GET BYTES PER LINE
    mbox[30] = 0x00040008;
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;
    mbox[34] = 0;
    if (mbox_call(8) && mbox[20] == 32 && mbox[28] != 0) {
		mbox[28] &= 0x3FFFFFFF;
		fb_width = mbox[10];
		fb_height = mbox[11];
		fb_pitch = mbox[33];
		fb_rgbp = mbox[24];
		fb = (uint8_t*)(mbox[28]);
	}
}
void drawPixel(uint32_t x, uint32_t y, uint32_t color){
    int offs = (y * fb_pitch) + (x * 4);
    *((uint32_t*)(fb + offs)) = color;
}
void uart_init(){
	mmio_write(UART0_CR, 0x00000000);
	mmio_write(GPPUD, 0x00000000);
	delay(150);
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
	mmio_write(GPPUDCLK0, 0x00000000);
	mmio_write(UART0_ICR, 0x7FF);
	#if PI_MOD>=3
		uint32_t r = (((uint32_t)(&mbox) & ~0xF) | 8);
		while ( mmio_read(MBOX_STATUS) & 0x80000000 ) { }
		mmio_write(MBOX_WRITE, r);
		while ( (mmio_read(MBOX_STATUS) & 0x40000000) || mmio_read(MBOX_READ) != r ) { }
	#endif
	mmio_write(UART0_IBRD, 1);
	mmio_write(UART0_FBRD, 40);
	mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
	mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
	mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
	}
void serial_putc(uint8_t c){
	while ( mmio_read(UART0_FR) & (1 << 5) ) { }
	mmio_write(UART0_DR, c);
}
void serial_puts(const char* str){
	for (size_t i = 0; str[i] != '\0'; i ++)
		serial_putc((uint8_t)str[i]);
}
const uint8_t hexchars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
void serial_puti(uint32_t value){
	uint8_t loc = 8;
	while (loc>0){
		serial_putc((uint8_t)hexchars[(value>>((loc*4)-4))%16]);
		loc--;
		}
	}
void serial_putb(uint8_t value){
	for (int8_t i=7; i>=0; i--){
		serial_putc((value & (1<<i)) ? '1' : '0');
	}
}
void fill_framebuffer(uint32_t color){
	for (uint32_t x=0, y=0; y<480; x++){
			drawPixel(x, y, color);
			if(x==720){
				y++;
				x=0;
			}
	}
}
// VIDEO CONTROLLERS
uint8_t *videoram;
uint32_t videoramlen;
#include "MDA.h"
uint8_t V = 'M';
struct FNS Fns;
void Kinterupt(){
  if (DEBUG) serial_puts("Handover: GUEST -> IBARMPC;\n");
}
int VM_main(uint32_t r0, uint32_t r1, uint32_t atags){
	serial_puts("IBARM PC version 0.1;\n");
	#if DEBUG==1
	serial_puts("Boot Options:");
	serial_puti(r0);
	serial_putc(',');
	serial_puti(r1);
	serial_putc(',');
	serial_puti(atags);
	serial_puts(";\n");
	#endif
	uart_init();
	frame_buffer_init(720, 480);
	MDA_show_string("IBARM PC", 324, 233, 0x00FF00, 0);
	MDA_show_string("version 0.1", 311, 247, 0x00FF00, 0);
	delay(1024*1024*1024);
	serial_puts("CONFIG;\n");
	// TODO: LOAD CONFIG
	if (V == 'V'); // VGA
	else if (V == 'C'); // CGA
	else MDA_init(); // MDA
	clear_framebuffer();
	MDA_show_string("Loading Kernel", 0, 0, 0x00FF00, 0);
        if (DEBUG) serial_puts("Handover: IBARMPC -> GUEST;\n");
	return 0;
}
