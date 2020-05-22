/*
* general.c
* MD407 library
*/
/* declarations goes to 'libMD407.h' */
#include "libMD407.h"
#include "usart_driver"
#include "keypad_driver"
#include "asciidisplay_driver"

/* Define variables here */
static char *heap_end;

PDEV_DRIVER_DESC device_table[MAX_FILENO] =
{
	&StdIn,
	&StdOut,
	&StdErr,
	&KeyPad,
	&AsciiDisplay,
};

/* Define functions here */
void crt_init() {
	extern char __bss_start;
	extern char __bss_end;
	extern char __heap_low;
	extern char __heap_top;
	char *s;
	s = &__bss_start;
	while( s < &__bss_end)
		*s++ = 0;
	s = &__heap_low;
	while( s < &__heap_top )
		*s++ = 0;
		
	for( int i = 0; i < MAX_FILENO; i++ )
	{
		fd = device_table[i];
		if( fd‐>init != 0)
		(void) fd‐>init( 0 );
	}
}

void crt_deinit(void)
{
	
}

char* _sbrk( int incr) {
	extern char __heap_low;
	extern char __heap_top;
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &__heap_low;
	}
	prev_heap_end = heap_end;
	if (heap_end + incr > &__heap_top) {
		/* Heap and stack collision */
		errno = ENOMEM;
		return (char *)-1;
	}
	heap_end += incr;
	return (char *) prev_heap_end;
}

int _close(int file)
{
	return -1;
}

int _fstat(int file, struct stat *st)
{
	st‐>st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file)
{
	return 1;
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int _open(const char *name, int flags, int mode)
{
	return -1;
}

int _read(int file, char *ptr, int len) {
	PDEV_DRIVER_DESC drvr;
	drvr = device_table[file];
	if(drvr == 0)
		return 0;
	return drvr‐>read(ptr,len);
}

int _write(int file, char *ptr, int len)
{
	PDEV_DRIVER_DESC drvr;
	drvr = device_table[file];
	if(drvr == 0)
		return 0;
	return drvr‐>write(ptr,len);
}