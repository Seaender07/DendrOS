/*
*
*	kernel.c
*	The entry point of the kernel
*
*/





#include "kernel.h"
#include "../drivers/drivers.h"





int main()
{
	clear_screen();
	print("Hello World!\n");
	
	print(kb_scan("Keyboard driver check: ", 35));
}
