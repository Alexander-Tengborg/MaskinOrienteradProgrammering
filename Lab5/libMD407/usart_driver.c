/*
 * libMD407
 * usart_driver.c
 * USART driver
 * Drivers for STDIN, STDOUT and STDERR
 */
#include "libMD407.h"

/* Define functions here */
static int usart_init(int initval );
static void usart_deinit(int deinitval);
static int usart_write(char *ptr, int len);
static int usart_read(char *ptr, int len);

DEV_DRIVER_DESC StdIn =
{
	{"stdin"},
	usart_init,
	usart_deinit,
	0,
	_isatty,
	0,
	0,
	0,
	0,
	usart_read
};

DEV_DRIVER_DESC StdOut =
{
	{"stdout"},
	usart_init,
	usart_deinit,
	0,
	_isatty,
	0,
	0,
	0,
	usart_write,
	0
};

DEV_DRIVER_DESC StdErr =
{
	{"stderr"},
	usart_init,
	usart_deinit,
	0,
	_isatty,
	0,
	0,
	0,
	usart_write,
	0
};

/* USART types and constants definitions */

typedef struct tag_usart
{
	volatile unsigned short sr;
	volatile unsigned short Unused0;
	volatile unsigned short dr;
	volatile unsigned short Unused1;
	volatile unsigned short brr;
	volatile unsigned short Unused2;
	volatile unsigned short cr1;
	volatile unsigned short Unused3;
	volatile unsigned short cr2;
	volatile unsigned short Unused4;
	volatile unsigned short cr3;
	volatile unsigned short Unused5;
	volatile unsigned short gtpr;
} USART;

#define USART1 ((USART *) 0x40011000)
#define USART2 ((USART *) 0x40004400)
#define USART3 ((USART *) 0x40004800)
#define USART4 ((USART *) 0x40004C00)

/* USART implementation */
static int usart_init(int initval)
{

}

static void usart_deinit(int deinitval)
{
	
}

static char _tstchar(void)
{
	if((USART1->sr & (1<<5))==0)
		return 0;
	return (char) USART1->dr;
}

static void _outchar(char c)
{
	while ((USART1->sr & (1<<7))==0);
	USART1->dr = (unsigned short) c;
}

static char _inchar(void)
{
	char c;
	while ((c=_tstchar())==0);
	return c;
}

static int usart_write(char *ptr, int len)
{
	int todo;
	for(todo = 0; todo < len; todo++)
	{
		_outchar(*ptr++);
	}
	return len;
}

static int usart_read(char *ptr, int len)
{
	char c;
	int todo;
	if(len == 0) return 0;
	for(todo = 1; todo <= len; todo++)
	{
		while((c = _tstchar()) == 0);
		*ptr++ = c;
	}
	return todo - 1;
} 