#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define RAND_MAX 255
#include <stdlib.h>

#include "common.h"

#include "Radzio_T6963_LIB/t6963c.h"
//#include "Radzio_T6963_LIB/graphic.h"
//#include "Fonts/mk_text.h"

uint8_t GLCD_Buffer[1024];

void _Init(void) {

}


FONT_INFO currentFont;

void setCurrentFont( const FONT_INFO * font ) {
	currentFont.heightPixels 		= pgm_read_byte(&font->heightPixels);
	currentFont.startChar 			= pgm_read_byte(&font->startChar);
	currentFont.interspacePixels	= pgm_read_byte(&font->interspacePixels);
	currentFont.spacePixels 		= pgm_read_byte(&font->spacePixels);
	currentFont.charInfo 			= (FONT_CHAR_INFO*)pgm_read_word(&font->charInfo);
	currentFont.data 				= (uint8_t*)pgm_read_word(&font->data);
}

void Buffer_to_GLCD(void) {

	GLCD_SetAddressPointer(GLCD_GRAPHIC_HOME);
	GLCD_WriteCommand(T6963_SET_ADDRESS_POINTER);

	for(uint16_t i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
		GLCD_WriteData(GLCD_Buffer[i]);
		GLCD_WriteCommand(T6963_DATA_WRITE_AND_INCREMENT);
	}
}

void Buffer_SetPixel(uint8_t x, uint8_t y, uint8_t color) {

	uint8_t tmp;
	uint16_t address;

	address = (x / 8) + ( y * GLCD_GRAPHIC_AREA );

	tmp = GLCD_Buffer[address];

	if(color)
	 tmp |= (1 <<  (7 - (x % 8)));
	else
	 tmp &= ~(1 <<  (7 - (x % 8)));

	GLCD_Buffer[address] = tmp;
}



uint8_t Div_w_add(uint8_t dzielna, uint8_t dzielnik) {
	uint8_t wynik, reszta;

	wynik = dzielna / dzielnik;
	reszta = dzielna % dzielnik;
	if ( reszta != 0 ) wynik = wynik + 1;	// Jezeli mamy 11 bitow / 8 = 1.375 bajta to musimy uzywc 2 bajtow i tak...

	return wynik;
}

void Font_Bitmap(FONT_CHAR_INFO * FONT, uint8_t cyfra, unsigned char x, unsigned char y) {
	uint8_t width = pgm_read_byte(&currentFont.charInfo[cyfra].widthBits);
	uint8_t height = pgm_read_byte(&MojFont_FontInfo.heightPixels);
	uint16_t offset = pgm_read_word(&currentFont.charInfo[cyfra].offset);
	uint16_t address = pgm_read_byte(&MojFont_FontInfo.data) + offset;

	for(uint8_t l = 0; l < height; l++) {
		for(uint8_t k = 0; k < Div_w_add(width,8); k++) {

			uint8_t bajt = pgm_read_byte(address++);

			for(uint8_t m=0; m<8; m++) {
				uint8_t pixel = (bajt >> (7-m)) & 0x01;
				Buffer_SetPixel(x+k*8+m, y+l, pixel);
			}
		}
	}
}

void Any_Bitmap(uint8_t * BITMAP, uint8_t X_POS, uint8_t Y_POS, uint8_t X_SIZE, uint8_t Y_SIZE) {

	//uint16_t j=0;

	for(uint8_t l = 0; l < Y_SIZE; l++) {
		for(uint8_t k = 0; k < Div_w_add(X_SIZE,8); k++) {

			uint8_t bajt = pgm_read_byte(BITMAP++);

			for(uint8_t m=0; m<8; m++) {
				uint8_t pixel = (bajt >> (7-m)) & 0x01;
				Buffer_SetPixel(X_POS+k*8+m, Y_POS+l, pixel);
			}
			//j++;
		}

	}
}


void digit(uint32_t liczba, uint8_t startx, uint8_t starty) {
	char string[10];
	char substring;
	uint8_t i=0;
	uint32_t cyfra;
	uint8_t width=0;

	ultoa(liczba,string,10);

	while (string[i]) {
		substring = string[i];
		cyfra = strtoul(&substring,NULL,0);
		width = pgm_read_byte(&currentFont.charInfo[cyfra].widthBits);
		Font_Bitmap(currentFont.charInfo, cyfra, startx+i*width, starty); //*width
		i++;
	}
}

int main(void) {
	_Init();
	// sei();

	GLCD_Initalize(); // Initalize LCD
	GLCD_ClearText(); // Clear text area
	GLCD_ClearCG(); // Clear character generator area
	GLCD_ClearGraphic(); // Clear graphic area

	setCurrentFont(&MojFont_FontInfo);

	Any_Bitmap((uint8_t *)MJS_Solutions,0,0,128,64);

	Buffer_to_GLCD();

	while(1) {

		/*static uint32_t licznik;
		digit(licznik++,0,10);
		Buffer_to_GLCD();*/
		//_delay_ms(10);

	}
return 1;
}
