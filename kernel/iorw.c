/*
*
*	iorw.c
*	Basic I/O functions
*
*/





#include "../include/include.h"





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
	for(int i = 0; i < bytes_no; i++)					// Repeat for {bytes_no} times
		*(dest + i) = *(source + i);					// Write to 'dest' (+ the offset of the current column) 'source' (+//)
}

char sel_bit(char byte, unsigned int selection)
{
	char comp = 1 << selection;						// Create a custom byte to compare with the given one
	return (byte & comp) >> selection;					// Return the selected byte as x*2^0 (>>)
}

char* cstr_copy(char** dest, char* source)
{
	switch(source[0])							// Check whether if the source string exists
	{
		case '\0':							// Return null if negative
			return "\0";
			break;

		default:							// Copy string if positive
			*dest = source;
			break;
	}

	return source;								// Return source string
}



char* bhex_cstr(unsigned char num)
{
	char* chex = "0x00";							// Initialize an empty model for a ascii hex byte
	
	chex[2] = (num >> 4) + ((num >> 4) < 0x0a ? 0x30 : 0x37);		// Write as first digit the 4th rsh of the number, plus the appropriate ascii offset
	chex[3] = (num & 0x0f) + ((num & 0x0f) < 0x0a ? 0x30 : 0x37);		// Write as second digit the bitwise and between the number and 0x0f, plus the appropriate ascii offset

	return chex;								// Return the modiefied array
}

char* char_cstr(char ch)
{
	char* str = "0";
	str[0] = ch;
	return str;
}
