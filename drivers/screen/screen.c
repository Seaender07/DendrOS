/*
*
*	screen.c
*	The screen driver's origin file
*
*/





#include "../drivers.h"





void print_char(char character, int row, int col, char attr_byte)	//	//	//	// Prints a character to a defined position with a defined color
{
	unsigned char *vidmem = (unsigned char *) VIDMEM;				// Create typewise alias of untyped constant VIDMEM
	int offset;



	if(!attr_byte)
		attr_byte = WH_BL;							// If no color schemes are provided, select the white-on-black one



	if(row >= 0 && col >= 0)
		offset = get_scrn_offset(row, col);					// Set as relative offset for the typing the relative offset of the given row and column (if positive)
	else
		offset = get_cursor();							// If negative, select as relative typing offset the current cursor position



	if(character == '\n')								// Escape char newline
	{
		int rows = offset / (2 * COLS);						// Get the number of the current row
		offset = get_scrn_offset(rows, 79);					// Set the offset to the ending of the line (increased later on)
	}
	else
	{
		vidmem[offset] = character;						// Print the character
		vidmem[offset + 1] = attr_byte;						// Set the color scheme byte
	}



	offset += 2;									// Select the next char byte (+ one for color scheme)
	offset = hand_scroll(offset);							// Ask if screen scrolling is needed (returns the beginning of the last line if true)

	set_cursor(offset);								// Set the cursor position at the specified relative offset
}



void print_at(char* message, int row, int col)	//	//	//	//	//	//	// Prints a string to a defined position
{
	if(row >= 0 && col >= 0)
		set_cursor(get_scrn_offset(row, col));					// Set the cursor position to the specified coords' relative offset (only if both are positive)

	for(int i = 0; message[i] != 0; i++)						// Iterate through 'message'
		print_char(message[i], row, col + i, WH_BL);				// Print the current character
}



void print(char* message)	//	//	//	//	//	//	//	//	// Prints a string to the current position of the cursor
{
	print_at(message, -1, -1);							// Print the message to the current position of the cursor
}



void clear_screen()	//	//	//	//	//	//	//	//	//	// Clears the screen
{
	char *vidmem = (char *)VIDMEM;							// Create a typewise alias of the untyped constant VIDMEM

	
	for(int i = 0; i < (2 * ROWS * COLS); i += 2)					// For every byte of the video memory
		*(vidmem + i) = 0;							// Fill the byte with 0 (NULL)

	set_cursor(get_scrn_offset(0, 0));						// Reset the cursor position to the beginning of the screen
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int get_scrn_offset(int row, int col)	//	//	//	//	//	//	//	// Returns the relative offset of the specified coordinates
{
	return (row * COLS + col) * 2;							// Return the relative offset from VIDMEM
}



int get_cursor()	//	//	//	//	//	//	//	//	//	// Returns the current position of the cursor
{
	o_write_byte(REG_SCRN_CTRL, 14);
	int offset = i_read_byte(REG_SCRN_DATA) << 8;					// Reads the high cursor register (shifts by 8 bits left)

	o_write_byte(REG_SCRN_CTRL, 15);
	offset += i_read_byte(REG_SCRN_DATA);						// Reads the low cursor register (adds the remainng 8 bits to the shifted ones)

	return offset * 2;								// Returns the actual relative offset (char slots have been counted, which is the half of that)
}



void set_cursor(int offset)	//	//	//	//	//	//	//	//	// Sets the position of the cursor at 'offset' (relative offset)
{
	offset /= 2;									// Makes the relative offset a char slot offset

	o_write_byte(REG_SCRN_CTRL, 14);
	o_write_byte(REG_SCRN_DATA, (unsigned char)(offset >> 8));			// Writes the first 8 bits to the high cursor register
	o_write_byte(REG_SCRN_CTRL, 15);
	o_write_byte(REG_SCRN_DATA, (unsigned char)(offset & 0x00FF));			// Writes the last 8 bits to the low cursor register
}



int hand_scroll(int cursor_offset)	//	//	//	//	//	//	//	// Returns whether if the screen has been scrolled
{
	if(cursor_offset < (2 * ROWS * COLS))
		return cursor_offset;							// No need of scrolling if the end is not reached, leaves with the given relative offset

	for(int i = 1; i < ROWS; i++)							// For every line copies COLS * 2 (char + color scheme) to the previous one
		mem_copy((char*)(get_scrn_offset(i, 0) + VIDMEM), (char*)(get_scrn_offset(i - 1, 0) + VIDMEM), 2 * COLS);

	char* last_line = (char*)(get_scrn_offset(ROWS - 1, 0) + VIDMEM);		// Absolute offset of the last line
	for(int i = 0; i < (COLS * 2); i++)						// For every byte of the row
		last_line[i] = 0;							// Fills the byte with 0

	cursor_offset -= 2 * COLS;							// Sets the relative offset of the cursor to the beginning of the last line (cleared)

	return cursor_offset;								// Returns the relative offset of the cursor
}
