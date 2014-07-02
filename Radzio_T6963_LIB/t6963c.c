//-------------------------------------------------------------------------------------------------
// Graphic LCD with Toshiba T6963 controller
// Copyright (c) Radoslaw Kwiecien, 2007r
// http://en.radzio.dxp.pl/t6963/
// Compiler : avr-gcc
//-------------------------------------------------------------------------------------------------
#include "T6963C.h"


//-------------------------------------------------------------------------------------------------
//
// Delay function
//	
//-------------------------------------------------------------------------------------------------
inline void delay(void) {
	volatile uint8_t i;
	for(i = 0; i < (F_CPU/1000000UL); i++) {
		asm("nop");
	}
}

//-------------------------------------------------------------------------------------------------
//
// Ports intalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_InitalizeInterface(void) {
GLCD_DATA_DDR = 0xFF;
GLCD_CTRL_DDR = ((1 << GLCD_WR) | (1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD) | (1 << GLCD_RESET) | (1 << GLCD_FS));
GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD) | (1 << GLCD_RESET) | (1 << GLCD_FS));
}

//-------------------------------------------------------------------------------------------------
//
// Reads dispay status
//
//-------------------------------------------------------------------------------------------------
inline uint8_t GLCD_CheckStatus(void) {

	uint8_t tmp;
	GLCD_DATA_DDR = 0x00;

	GLCD_CTRL_PORT &= ~((1 << GLCD_RD) | (1 << GLCD_CE));
	delay();
	tmp = GLCD_DATA_PIN;
	GLCD_DATA_DDR = 0xFF;
	GLCD_CTRL_PORT |= ((1 << GLCD_RD) | (1 << GLCD_CE));

	return tmp;
}

//-------------------------------------------------------------------------------------------------
//
// Writes instruction 
//
//-------------------------------------------------------------------------------------------------
inline void GLCD_WriteCommand(unsigned char command) {

	while(!(GLCD_CheckStatus()&0x03));
	GLCD_DATA_PORT = command;

	GLCD_CTRL_PORT &= ~((1 << GLCD_WR) | (1 << GLCD_CE));
	//delay();
	GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_CE));
}

//-------------------------------------------------------------------------------------------------
//
// Writes data
//
//-------------------------------------------------------------------------------------------------
inline void GLCD_WriteData(unsigned char data) {

	while(!(GLCD_CheckStatus()&0x03));
	GLCD_DATA_PORT = data;

	GLCD_CTRL_PORT &= ~((1 << GLCD_WR) | (1 << GLCD_CE) | (1 << GLCD_CD));
	//delay();
	GLCD_CTRL_PORT |= ((1 << GLCD_WR) | (1 << GLCD_CE) | (1 << GLCD_CD));
}

//-------------------------------------------------------------------------------------------------
//
// Reads data
//
//-------------------------------------------------------------------------------------------------
inline unsigned char GLCD_ReadData(void) {

	uint8_t tmp;
	while(!(GLCD_CheckStatus()&0x03));
	GLCD_DATA_DDR = 0x00;

	GLCD_CTRL_PORT &= ~((1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD));
	//delay();
	tmp = GLCD_DATA_PIN;
	GLCD_CTRL_PORT |= ((1 << GLCD_RD) | (1 << GLCD_CE) | (1 << GLCD_CD));
	GLCD_DATA_DDR = 0xFF;

	return tmp;
}

//-------------------------------------------------------------------------------------------------
//
// Sets address pointer for display RAM memory
//
//-------------------------------------------------------------------------------------------------
inline void GLCD_SetAddressPointer(unsigned int address) {
	GLCD_WriteData(address & 0xFF);
	GLCD_WriteData(address >> 8);
	GLCD_WriteCommand(T6963_SET_ADDRESS_POINTER);
}





//-------------------------------------------------------------------------------------------------
//
// Clears text area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearText(void)
{
int i;
GLCD_SetAddressPointer(GLCD_TEXT_HOME);

for(i = 0; i < GLCD_TEXT_SIZE; i++)
  {
  GLCD_WriteDisplayData(0);
  }
}
//-------------------------------------------------------------------------------------------------
//
// Clears characters generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearCG(void)
{
unsigned int i;
GLCD_SetAddressPointer(GLCD_EXTERNAL_CG_HOME);

for(i = 0; i < 256 * 8; i++)
  {
  GLCD_WriteDisplayData(0);
  }
}

//-------------------------------------------------------------------------------------------------
//
// Clears graphics area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_ClearGraphic(void) {

	int i;
	GLCD_SetAddressPointer(GLCD_GRAPHIC_HOME);
	for(i = 0; i < GLCD_GRAPHIC_SIZE; i++) {
		GLCD_WriteDisplayData(0x00);
	}
}

//-------------------------------------------------------------------------------------------------
//
// Writes a single character (ASCII code) to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteChar(char charCode)
{
GLCD_WriteDisplayData(charCode - 32);
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteString(char * string)
{
while(*string)
  {
  GLCD_WriteChar(*string++);
  }
}
//-------------------------------------------------------------------------------------------------
//
// Writes null-terminated string from program memory to display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteStringPgm(char * string)
{
char ch;
while((ch = pgm_read_byte(string++)))
  {
  GLCD_WriteChar(ch);
  }
}

void GLCD_WriteInt(int16_t data) {
	char string[5];
	itoa(data,string,10);
	GLCD_WriteString(string);
}




//-------------------------------------------------------------------------------------------------
//
// Sets display coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_TextGoTo(unsigned char x, unsigned char y) {

	unsigned int address;
	address = GLCD_TEXT_HOME + x + (GLCD_TEXT_AREA * y);
	GLCD_SetAddressPointer(address);
}

//-------------------------------------------------------------------------------------------------
//
// Writes single char pattern to character generator area of display RAM memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_DefineCharacter(unsigned char charCode, unsigned char * defChar)
{
unsigned int address;
unsigned char i; 

address = GLCD_EXTERNAL_CG_HOME + (8 * charCode);

GLCD_SetAddressPointer(address);

for(i = 0; i < 8 ; i++)
  {
  GLCD_WriteDisplayData(*(defChar + i));
  }
}

//-------------------------------------------------------------------------------------------------
//
// Set (if color==1) or clear (if color==0) pixel on screen
//
//-------------------------------------------------------------------------------------------------
void GLCD_SetPixel(unsigned char x, unsigned char y, unsigned char color)
{
unsigned char tmp;
unsigned int address;

address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);

GLCD_SetAddressPointer(address);

GLCD_WriteCommand(T6963_DATA_READ_AND_NONVARIABLE);
tmp = GLCD_ReadData();

if(color)
 tmp |= (1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));
else
 tmp &= ~(1 <<  (GLCD_FONT_WIDTH - 1 - (x % GLCD_FONT_WIDTH)));

GLCD_WriteDisplayData(tmp);

}
//-------------------------------------------------------------------------------------------------
//
// Writes display data and increment address pointer
//
//-------------------------------------------------------------------------------------------------
void GLCD_WriteDisplayData(unsigned char x)
{
GLCD_WriteData(x);
GLCD_WriteCommand(T6963_DATA_WRITE_AND_INCREMENT);
}
//-------------------------------------------------------------------------------------------------
//
// Sets graphics coordinates
//
//-------------------------------------------------------------------------------------------------
void GLCD_GraphicGoTo(unsigned char x, unsigned char y) {
unsigned int address;
address = GLCD_GRAPHIC_HOME + (x / GLCD_FONT_WIDTH) + (GLCD_GRAPHIC_AREA * y);
GLCD_SetAddressPointer(address);
}
//-------------------------------------------------------------------------------------------------
//
// Displays bitmap from program memory
//
//-------------------------------------------------------------------------------------------------
void GLCD_Bitmap(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
unsigned char i, j;

for(j = 0; j < height; j++)
{
GLCD_GraphicGoTo(x, y + j);
for(i = 0; i < width/GLCD_FONT_WIDTH; i++)
  {
  GLCD_WriteDisplayData(pgm_read_byte(bitmap + i + (GLCD_GRAPHIC_AREA * j))); 	
  }
}
}

void GLCD_Bitmap_digit(unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char width, unsigned char height) {
	for(uint8_t l= 0; l < height; l++) {
		GLCD_GraphicGoTo(x, y + l);

		for(uint8_t k = 0; k < (width/GLCD_FONT_WIDTH); k++) {
			GLCD_WriteDisplayData( pgm_read_byte(bitmap ++) );
		}
	}
}


//-------------------------------------------------------------------------------------------------
//
// Display initalization
//
//-------------------------------------------------------------------------------------------------
void GLCD_Initalize(void)
{
GLCD_InitalizeInterface();

GLCD_CTRL_PORT &= ~(1 << GLCD_RESET);
_delay_ms(1);
GLCD_CTRL_PORT |= (1 << GLCD_RESET);

#if (GLCD_FONT_WIDTH == 8)
GLCD_CTRL_PORT &= ~(1 << GLCD_FS);
#else
GLCD_CTRL_PORT |= (1 << GLCD_FS);
#endif

GLCD_WriteData(GLCD_GRAPHIC_HOME & 0xFF);
GLCD_WriteData(GLCD_GRAPHIC_HOME >> 8);
GLCD_WriteCommand(T6963_SET_GRAPHIC_HOME_ADDRESS);

GLCD_WriteData(GLCD_GRAPHIC_AREA);
GLCD_WriteData(0x00);
GLCD_WriteCommand(T6963_SET_GRAPHIC_AREA);

GLCD_WriteData(GLCD_TEXT_HOME);
GLCD_WriteData(GLCD_TEXT_HOME >> 8);
GLCD_WriteCommand(T6963_SET_TEXT_HOME_ADDRESS);

GLCD_WriteData(GLCD_TEXT_AREA);
GLCD_WriteData(0x00);
GLCD_WriteCommand(T6963_SET_TEXT_AREA);

GLCD_WriteData(GLCD_OFFSET_REGISTER);
GLCD_WriteData(0x00);
GLCD_WriteCommand(T6963_SET_OFFSET_REGISTER);

GLCD_WriteCommand(T6963_DISPLAY_MODE  | T6963_GRAPHIC_DISPLAY_ON   | T6963_TEXT_DISPLAY_ON /*| T6963_CURSOR_DISPLAY_ON*/);

GLCD_WriteCommand(T6963_MODE_SET);
}



/* Zgodnosc z biblioteka Mirka*/
void Draw_pixel(void) {
	GLCD_WriteData(1);
}

void Draw_bk_pixel(void) {
	GLCD_WriteData(0);
}

/*void Set_active_window( int X1, int Y1, int X2, int Y2 ) {
	GLCD_WriteCommand(0x2a);
	GLCD_WriteDisplayData(X1>>8);
	GLCD_WriteDisplayData(X1);
	GLCD_WriteDisplayData(X2>>8);
	GLCD_WriteDisplayData(X2);

	GLCD_WriteCommand(0x2b);
	GLCD_WriteDisplayData(Y1>>8);
	GLCD_WriteDisplayData(Y1);
	GLCD_WriteDisplayData(Y2>>8);
	GLCD_WriteDisplayData(Y2);
}*/

void put_pixel(int x, int y) {
	GLCD_SetPixel(x,y,1);
}

void put_bk_pixel(int x, int y) {
	GLCD_SetPixel(x,y,0);
}
