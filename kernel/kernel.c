/*
*
*	kernel.c
*	The entry point of the kernel
*
*/





#include "../include/include.h"





int main()
{
	clear_screen();
	print("Hello World!\n");

	print(kb_scan("Keyboard driver check: ", 35));
}
