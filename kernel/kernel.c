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

	char* greet;
	kb_scan("Keyboard driver check: ", 35, &greet);
	print(greet); print("\n");

	if(cstr_comp(greet, "Davide"))
	{
		print("Hello!\n");
	}
	else
	{
		print("Not you.\n");
	}
}
