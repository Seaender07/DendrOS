/*
*
*	keyboard.c
*	The keyboard driver's source file
*
*/





#include "../drivers.h"





static char ctrl_status = 0, shift_status = 0, capsl_status = 0, alt_status = 0, meta_status = 0;			// Modifier keys' status indicators





const unsigned char kb_keys[] = {											// Keyboard's keys' scancodes
	0x29, 	0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,		0x0c, 0x0d, 0x0e,		// First row
	0x0f, 	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,		0x1a, 0x1b, 0x2b, 0x1c,		// Second row
	      	0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26,			0x27, 0x28,			// Third row
	      	0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32,				0x33, 0x34, 0x35,		// Fourth row
	0x39,														// Space bar
	0xff														// Default return value (no key match found)
};

const unsigned char kb_modifiers[] = {											// Keyboard's modifier keys
	0x38,														// ALTs
	0x3a,														// Caps Lock
	0x2a, 0x36,													// Shifts
	0x1d,														// CTRLs
	0x5b, 0x5c,													// Metas
	0x48, 0x50,													// Up and down arrows
	0x4b, 0x4d,													// Left and right arrows
	0xff
};

const char kb_ascii_lk[] = {												// American lowercase keyboard layout
	'`' , 	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',			'-', '=' , '\b',
	'\t', 	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',			'[', ']' , '\\', '\n',
	      	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',				';', '\'',
	      	'z', 'x', 'c', 'v', 'b', 'n', 'm',					',', '.' , '/' ,
	' '
};

const char kb_ascii_uk[] = {												// American uppercase keyboard layout
	'~' , 	'!', '@', '#', '$', '%', '^', '&', '*', '(', ')',			'_', '+' , '\b',
	'\t', 	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',			'{', '}' , '|' , '\n',
	      	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',				':', '\"',
	      	'Z', 'X', 'C', 'V', 'B', 'N', 'M',					'<', '>' , '?' ,
	' '
};

const char kb_fkascii_mods[] = {											// Return values for modifiers' keypresses
	0x0a,														// ALTs
	0x0b,														// Caps Lock
	0x0c, 0x0c,													// Shifts
	0x0d,														// CTRLs
	0x0e, 0x0e,													// Metas
	0x10, 0x11,													// Up and down arrows
	0x12, 0x13													// Left and right arrows
};





char ascii_key(unsigned char kb_key)	//	//	//	//	//	//	//	// Converts keyboard's scancodes in ascii characters
{
	int i = 0;
	int im = 0;
	int kb_m = 0;									// True if a modifier key is pressed

	while(i < (sizeof(kb_keys) / 1 - 1))						// Find the correct index of the scancode in the regular keys' array (until the end is reached)
	{
		if(kb_keys[i] == kb_key)						// Break if the indexed value matches
			break;
		i++;									// Increase the index
	}
	
	if(kb_keys[i] == 0xff)								// If the scancode doesn't match any of the known keys
	{
		im = 0;									// New index 'im'
		while(im < (sizeof(kb_modifiers) / 1 - 1))				// Find the correct index of the scancode in the modifier keys' array (until the end is reached)
		{
			if(kb_modifiers[im] == kb_key)					// Set the modifier flag on and break if the indexed value matches
			{
				kb_m = 1;
				break;
			}
			im++;								// Increase the index
		}
	}

	switch(kb_m)									// Start the return value's choice from the modifier flag
	{
		case 0:									// No modifiers were used
			switch(kb_keys[i])
			{
				case 0xff:						// If no matching keys, just return an error
					return 0x01;
					break;

				default:						// Else
					if(shift_status == 1 || capsl_status == 1)	// Return the uppercase ascii if some caps' modifier is on
						return kb_ascii_uk[i];
					else						// Otherwise the lowercase ascii
						return kb_ascii_lk[i];
					break;
			}
			break;

		case 1:									// A modifier was used
			switch(kb_modifiers[im])
			{
				case 0xff:						// If no matching keys, just return an error
					return 0x01;
					break;

				default:						// Otherwise return a dummy char code
					return kb_fkascii_mods[im];
					break;
			}
			break;

		default:								// The modifier flag has an unknown value
			print("Error: kb_m not set!\n");				// Print an error message
			return 0xff;
	}

	return 0x01;									// Return an error (no regular condition brings here) 
}





char kb_i_char(int canbs, int ifprint)	//	//	//	//	//	//	//	// Gets a character from the keyboard device
{
	unsigned char asciino;								// Holds the scancode
	char ascii;									// Holds the ascii code

kb_i_char_start:

	o_write_byte(KB_I, 0xc0);							// Set the keyboard controller's mode to input read
	
	unsigned char iKcsr = i_read_byte(KB_I);					// Store the read scancode

	do										// Do this until the scancode is different from the default one (keypress happened)
	{
		asciino = i_read_byte(KB_I);						// Store here the valid scancode
	} while(iKcsr == asciino);

	if(asciino < 0x80)								// If the scancode is a keypress (asciino >= 0x80 are key releases)
	{
		ascii = ascii_key(asciino);						// Decode the scancode into an ascii char

		switch(ascii)								// Sort the ascii char
		{
			case '\n':							// Newline char: print and return it
				print(char_cstr(ascii));
				return ascii;
				break;
		
			case '\b':							// Backspace char
				if(ifprint > 0 && canbs > 0)
					print(char_cstr(ascii));			// Print if print mode is on and the current screen offset is greater than the initial one
				else if(ifprint <= 0 && canbs > 0)
					return ascii;					// Return it if print mode is on but it could be performed
				else
					goto kb_i_char_start;				// If none of this is true, just restart the function
				break;

			case 0x0b:							// Capslock keypress
				capsl_status = (capsl_status == 0 ? 1 : 0);		// Turn on if it's off and vice versa
				break;

			case 0x0c:							// Shift keypress
				shift_status = 1;					// Set the shift status flag on
				break;

			case 0x10:							// TODO Upward arrow
				break;

			case 0x11:							// TODO Downward arrow
				break;

			case 0x12:							// TODO Left arrow
				//set_cursor(get_cursor() - 1);
				break;

			case 0x13:							// TODO Right arrow
				//set_cursor(get_cursor() + 1);
				break;

			case 0x01:							// Unknown scancode
				goto kb_i_char_start;
				break;
		}
		
		if(ascii <= 0x1f && ascii != '\t' && ascii != '\b')			// If 'ascii' is a non-printable char and it is not a tab or a backspace
			goto kb_i_char_start;						// Restart the function
		else if(ascii >= 0x20)							// If 'ascii' is a printable char
		{
			if(ifprint > 0)
				print(char_cstr(ascii));				// Print only if print mode is on
		}
	}
	else										// If 'asciino' holds a keypress
	{
		switch(asciino)								// Sort 'asciino'
		{
			case 0x9d:							// CTRL key release
				ctrl_status = 0;					// Set the flag to 0
				break;
	
			case 0xaa: case 0xb6:						// Shift key release
				shift_status = 0;					// Set the flag to 0
				break;
	
			case 0xb8:							// ALT key release
				alt_status = 0;						// Set the flag to 0
				break;
	
			case 0xdb: case 0xdc:						// Meta key release
				meta_status = 0;					// Set the flag to 0
				break;
	
			default:							// If no matches found, leave
				break;
		}

		goto kb_i_char_start;							// Anyhow restart the function (key release do not count as chars)
	}

	return ascii;									// Return the read char
}





char* kb_scan(char* message, unsigned int charno)	//	//	//	//	//	// Gets a C string from the keyboard device
{
	if(message)
		print(message);								// If some input message is given, print it

	int soffs = get_cursor();							// Get the screen offset of the beginning
	int coffs;
	char pscanned;

	for(int i = 0; i < charno; i++)							// For many times as the no. of chars requested
	{
		coffs = get_cursor();							// Get the current screen offset
		pscanned = kb_i_char((coffs - soffs), 1);				// Store the scanned char in 'pscanned'

		switch(pscanned)							// Sort pscanned
		{
			case '\n':							// Go to end if a newline is passed
				goto kbsc_swend;
				break;

			case '\b':							// Backspace char
				i -= 2;							// Go back by one char (-2 because the index will increase by 1 on the next execution of 'for')
				break;

			default:							// Leave if no matches are found
				break;
		}

		if(i == (charno - 1))							// If this is the last execution of 'for' (last character)
		{
			do								// Do this until a newline is returned (then exit, 'for' won't be executed anymore)
			{
				pscanned = kb_i_char((coffs - soffs), 0);		// Get a character
				if(pscanned == '\b')					// If 'pscanned' holds a backspace
				{
					print("\b");					// Print the backspace
					i--;						// Decrease the index by one
					break;
				}
			} while(pscanned != '\n');
		}
	}

kbsc_swend:

	int foffs = get_cursor();							// Get the screen offset of the end

	char* sstr = scrn_getstr(soffs, (foffs - soffs) / 2);				// Get the written screen memory, from the initial to the final offset
	static char rscan[0x100];							// Create a static C string to store the clean result

	for(int i = 0; sstr[i] != '\0'; i++)						// Transfer the string to the static one until a NULL char is reached
		rscan[i] = sstr[i];

	return rscan;									// Return the scanned string
}
