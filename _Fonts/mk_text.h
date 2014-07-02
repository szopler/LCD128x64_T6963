/*
 * tft_text.h
 *
 *  Created on: 2011-09-19
 *       Autor: Miros³aw Kardaœ
 */

#include "mk_fonts.h"

#ifndef TFT_TEXT_H_
#define TFT_TEXT_H_

extern FONT_INFO currentFont;

void setCurrentFont( FONT_INFO * font );

void tft_puts( int x, int y, char * s);
void tft_puts_P( int x, int y, char * s);

void tft_mputs( int x, int y, char * s);

int text_len(char *s);
int text_len_P(char *s);

void tft_putint(int x, int y, int val, uint32_t pen, uint32_t brush);
void tft_puthex(int x, int y, uint32_t val, uint32_t pen, uint32_t brush);

#endif /* TFT_TEXT_H_ */
