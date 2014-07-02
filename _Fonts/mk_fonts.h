/*
*  mk_fonts.h
*  Created on: 2014-06-29 15:21:55
*       Autor: Miros³aw Kardaœ
*       www.atnel.pl
*/
#ifndef MK_FONTX_H_
#define MK_FONTX_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
// ==========================================================================
// structure definition
// ==========================================================================

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array

} FONT_CHAR_INFO;

// Describes a single font
typedef struct
{
	uint8_t heightPixels;	// height, in pages (8 pixels), of the font's characters
	uint8_t startChar;	// the first character in the font (e.g. in charInfo and data)
	uint8_t interspacePixels;	// number of pixels of interspace between characters
	uint8_t spacePixels;	// number of pixels of space character
	FONT_CHAR_INFO * charInfo;	// pointer to array of char information
	uint8_t* data;			// pointer to generated array of character visual representation
    char * filename;    // (Pointer) Font filename saved on SD card or 0 (null) otherwise

} FONT_INFO;

// Font data for Comic Sans MS 12pt
extern const uint8_t PROGMEM ComicSansMS12ptBitmaps[];
extern const FONT_CHAR_INFO PROGMEM ComicSansMS12ptDescriptors[];
extern const FONT_INFO PROGMEM ComicSansMS12ptFontInfo;
#endif /* MK_FONTX_H_ */
