#include "symbols.h"
#include <stdint.h>
#include "Resources/MDA.h"
void MDA_init(){
	videoramlen = 4000;
	videoram = (uint8_t*)0x18000;
}
void MDA_draw_char(uint8_t c, uint32_t x, uint32_t y, uint32_t color, uint32_t bgcolor){
	for (uint8_t off = 0; off<14; off++){
		for (int8_t xoff = 0; xoff<8; xoff++){
			drawPixel(x+xoff, y+off, (MDA_FONT[c*16+off] & (1<<(7-xoff))) ? color : bgcolor);
		}
		drawPixel(x+8, y+off, (MDA_FONT[c*16+(off>=8 ? 15 : 14)] & (1<<(7-(off%8)))) ? color : bgcolor);
	}
}
void MDA_show_string(const char* str, uint32_t x, uint32_t y, uint32_t color, uint32_t bgcolor){
	for (size_t i = 0; str[i] != '\0'; i ++)
		MDA_draw_char((uint8_t)str[i], x+i*9, y, color, bgcolor);
}
void MDA_update(){
	clear_framebuffer();
	for (uint8_t chr = 0; chr<2000; chr++){
		// TODO: ADD FLAGS
		MDA_draw_char(videoram[chr*2], (chr%80)*9, (chr/80)*14, 0x00FF00, 0);
	}
}
