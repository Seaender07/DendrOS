/*
*
*	iorw.c
*	Basic I/O functions
*
*/





#include "kernel.h"





unsigned char i_read_byte(unsigned short port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));			// Reads from 'port' (dx) the 'result' (al)
	return result;
}

void o_write_byte(unsigned short port, unsigned char data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));			// Writes to 'port' (dx) the 'data' (al)
}

unsigned short i_read_word(unsigned short port)
{
	unsigned short result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));			// Reads from 'port' (dx) the 'result' (al)
	return result;
}

void o_read_word(unsigned short port, unsigned short data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));			// Writes to 'port' (dx) the 'data' (al)
}



void mem_copy(char* source, char* dest, int bytes_no)
{
	for(int i = 0; i < bytes_no; i++)			// Repeat for {bytes_no} times
		*(dest + i) = *(source + i);			// Write to 'dest' (+ the offset of the current column) 'source' (+//)
}
