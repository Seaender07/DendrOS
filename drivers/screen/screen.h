/*
*
*	screen.h
*	The screen driver's header file
*
*/





#define VIDMEM 0xb8000			// Video memory address
#define ROWS 25				// Number of rows
#define COLS 80				// Number of columns

#define WH_BL 0x0f			// White on black color scheme
#define HTAB_SZ 3			// Horizontal tab size
#define VTAB_SZ	3			// Vertical tab size

#define REG_SCRN_CTRL 0x03d4		// Screen control register
#define REG_SCRN_DATA 0x03d5		// Screen data register





int get_scrn_offset(int row, int col);			// Returns the relative offset of the specified coordinates
int get_cursor();					// Returns the current position of the cursor
void set_cursor(int offset);				// Sets the position of the cursor at offset
int hand_scroll(int cursor_offset);			// Returns whether if the screen has been scrolled

void print_char(char character, int row, int col, char attr_byte);		// Prints a character to a defined position with a defined color
void print_at(char* message, int row, int col);					// Prints a string to a defined position
void print(char* message);							// Prints a string to the current position of the cursor
void clear_screen();								// Clears the screen
