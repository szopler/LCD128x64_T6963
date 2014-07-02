/*
 * tft_text.c
 *
 *  Created on: 2011-09-19
 *       Autor: Miros³aw Kardadaœ
 */
//#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "../Radzio_T6963_LIB/t6963c.h"
#include "mk_text.h"

FONT_INFO currentFont;

void setCurrentFont( FONT_INFO * font ) {
	currentFont.heightPixels 		= pgm_read_byte(&font->heightPixels);
	currentFont.startChar 			= pgm_read_byte(&font->startChar);
	currentFont.interspacePixels	= pgm_read_byte(&font->interspacePixels);
	currentFont.spacePixels 		= pgm_read_byte(&font->spacePixels);
	currentFont.charInfo 			= (FONT_CHAR_INFO*)pgm_read_word(&font->charInfo);
	currentFont.data 				= (uint8_t*)pgm_read_word(&font->data);
	currentFont.filename 			= (char*)pgm_read_word(&font->filename);
}

static void send_glyph_byte(uint8_t bajt, uint8_t width) {
	uint8_t i;
	for(i=0; i<width; i++ ) {
		if(bajt&0x80) Draw_pixel();
		else Draw_bk_pixel();
		bajt<<=1;
	}
}

static void send_font_bitmap(int xPixel, int yPixel, uint8_t *glyph, int glyphHeight, int glyphWidth ) {

	uint8_t i, j, k, bajt;
	uint16_t p=0;

	//Set_active_window(xPixel, yPixel, xPixel+glyphWidth-1, yPixel+glyphHeight-1);
	//GLCD_WriteCommand(0x2c);
	//GLCD_GraphicGoTo(x, y + j);

	for(i=0; i<glyphHeight; i++ ) {
		for(j=0, k=0; j<glyphWidth; j+=8, k++) {
			bajt = pgm_read_byte( &glyph[ p++ ] );
			if( ((k+1)*8)<=glyphWidth ) send_glyph_byte(bajt, 8);
			else send_glyph_byte(bajt, glyphWidth-(k*8));
		}
	}
}


void tft_mputs( int x, int y, char * s) {

	uint8_t gH, gW, gS, gIS;
	uint16_t offset;
	uint8_t startChar = currentFont.startChar;
	uint8_t * glyph = (uint8_t*)currentFont.data;

	//Set_color32(color);
	//Set_bk_color32(bk_color);
	gH = currentFont.heightPixels;
	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( *s ) {
		if( *s > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ *s - startChar  ].widthBits );

			offset = pgm_read_word( &currentFont.charInfo[ *s - startChar  ].offset);

			send_font_bitmap(x, y, glyph+offset, gH, gW );
			x = x + gW + gIS;

		} else {
			//Set_active_window(x,y,x+gS-1,y+gH-1);
			//GLCD_WriteCommand(0x2c);
			for(offset=0;offset<gS*gH;offset++) {
				Draw_bk_pixel();
				Draw_bk_pixel();
				Draw_bk_pixel();
			}
			x+=gS;
		}
		s++;
	}

	CX=x;
	CY=y;
}

void tft_puts( int x, int y, char * s) {

	y+=frame_ptr;

	if( !currentFont.filename ) {
		tft_mputs(x,y, s);
		return;
	}
/*#if USE_PETIT_FAT == 0
	else return;
#endif*/
}

void tft_mputs_P( int x, int y, char * s) {

	char c;
	uint8_t gH, gW, gS, gIS;
	uint16_t offset;
	uint8_t startChar = currentFont.startChar;
	uint8_t * glyph = (uint8_t*)currentFont.data;


	//Set_color32(color);
	//Set_bk_color32(bk_color);
	gH = currentFont.heightPixels;
	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( (c=pgm_read_byte(s)) ) {
		if( c > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ c - startChar  ].widthBits );

			offset = pgm_read_word( &currentFont.charInfo[ c - startChar  ].offset);

			send_font_bitmap(x, y, glyph+offset, gH, gW );
			x = x + gW + gIS;

		} else {
			//Set_active_window(x,y,x+gS-1,y+gH-1);
			//GLCD_WriteCommand(0x2c);
			for(offset=0;offset<gS*gH;offset++) {
				Draw_bk_pixel();
				Draw_bk_pixel();
				Draw_bk_pixel();
			}
			x+=gS;
		}
		s++;
	}

	CX=x;
	CY=y;
}

int text_len(char *s) {

	int x=0;
	uint8_t gH, gW, gS, gIS;
	uint8_t startChar = currentFont.startChar;

	gH = currentFont.heightPixels;
	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( *s ) {
		if( *s > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ *s - startChar  ].widthBits );
			x = x + gW + gIS;
		} else x+=gS;
		s++;
	}

	return x;
}

int text_len_P(char *s) {

	char c;
	int x=0;
	uint8_t gH, gW, gS, gIS;
	uint8_t startChar = currentFont.startChar;

	gH = currentFont.heightPixels;
	gIS = currentFont.interspacePixels;
	gS = currentFont.spacePixels;

	while( (c=pgm_read_byte(s)) ) {
		if( c > ' ') {
			gW = pgm_read_byte( &currentFont.charInfo[ c - startChar  ].widthBits );
			x = x + gW + gIS;
		} else x+=gS;
		s++;
	}

	return x;
}



void tft_puts_P( int x, int y, char * s) {

	y+=frame_ptr;

	if( !currentFont.filename ) {
		tft_mputs_P(x,y,s);
		return;
	}
}

void tft_putint(int x, int y, int val, uint32_t pen, uint32_t brush) {
	char bufor[17];

	itoa(val, bufor, 10);
	tft_puts(x, y, bufor );
}

void tft_puthex(int x, int y, uint32_t val, uint32_t pen, uint32_t brush) {
	char bufor[17];

	memset(bufor, 0, 17);
	ltoa(val, bufor, 16);
	tft_puts(x, y, bufor);
}
