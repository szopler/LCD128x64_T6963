#ifndef COMMON_H_
#define COMMON_H_

/* Macro definitions */
#define sbi(byte,bit) byte |=  (1<<bit)
#define cbi(byte,bit) byte &= ~(1<<bit)
#define bit(byte,n) (byte>>n)&1

#ifndef NULL
#define NULL   ((void *) 0)
#endif


#endif /* COMMON_H_ */
