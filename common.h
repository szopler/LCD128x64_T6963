#ifndef COMMON_H_
#define COMMON_H_

/* Macro definitions */
#define sbi(byte,bit) byte |=  (1<<bit)
#define cbi(byte,bit) byte &= ~(1<<bit)
#define bit(byte,n) (byte>>n)&1

#ifndef NULL
#define NULL   ((void *) 0)
#endif

const int SIN_every_22_5_deg[16] = { 0, 383, 707, 383, 1000, 383, 707, 383, 0, -383, -707, -383, -1000, -383, -707, -383 };
const int TAN_every_22_5_deg[16] = { 0, 414, 1000, 1414, 0, -1414, -1000, -414, 0, 414, 1000, 1414, 0, -1414, -1000, -414 };

typedef struct {
	int16_t end_X;
	int16_t end_Y;
} krechy;

const krechy kreski[] = {
		{ 107, 5 },
		{ 112, 6 },
		{ 115, 8 },
		{ 119, 12 },
		{ 122, 15 },
		{ 119, 18 },
		{ 115, 22 },
		{ 112, 24 },
		{ 107, 25 },
		{ 102, 24 },
		{ 99, 22 },
		{ 95, 18 },
		{ 92, 15 },
		{ 95, 12 },
		{ 99, 8 },
		{ 102, 6 },
		{ 107, 5}
};

#endif /* COMMON_H_ */
