/*
*
*	include.h
*	The main include directive
*
*/



#ifndef NULL
#	define NULL 0x00
#endif

#ifndef	BUSIO
#	define BUSIO
#	include "../kernel/kernel.h"
#endif

#ifndef DRIVERS
#	define DRIVERS
#	include "../drivers/drivers.h"
#endif
