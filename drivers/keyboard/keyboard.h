/*
*
*	keyboard.h
*	The keyboard driver's header file
*
*/





#define KB_I 0x60			// Keyboard controller command byte (mnemonic input)
#define KB_O 0x60			// Keyboard controller command byte (mnemonic output)
#define KB_CTRL 0x64			// Keyboard controller status register





char ascii_key(unsigned char kb_key);					// Converts keyboard's scancodes in ascii characters
char kb_i_char(int canbs, int ifprint);					// Gets a character from the keyboard device
char* kb_scan(char* message, unsigned int charno, char** storage);	// Gets a C string from the keyboard device
