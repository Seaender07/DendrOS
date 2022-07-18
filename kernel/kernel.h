/*
*
*	kernel.h
*	Basic I/O functions
*
*/





unsigned char i_read_byte(unsigned short port);				// Read a byte from a specified port
void o_write_byte(unsigned short port, unsigned char data);		// Write a byte to a specified port

unsigned short i_read_word(unsigned short port);			// Read a word from a specified port
void o_read_word(unsigned short port, unsigned short data);		// Write a word to a specified port

void mem_copy(char* source, char* dest, int bytes_no);			// Copy x bytes from a specified location to another
char sel_bit(char byte, unsigned int selection);			// Returns a single specified bit from a whole byte

char* bhex_cstr(unsigned char num);					// Writes a hex byte as a c string
char* char_cstr(char ch);
