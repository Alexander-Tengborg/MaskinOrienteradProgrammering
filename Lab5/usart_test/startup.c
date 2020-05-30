/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

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

void _outchar( char c )
{
	while (( USART1->sr & (1<<7) )==0);
	USART1->dr = (unsigned short) c;
}

char _tstchar( void )
{
	if( (USART1->sr & (1<<5) )==0)
		return 0;
	return (char) USART1->dr;
}

char _inchar( void )
{
	char c;
	while ( (c=_tstchar() )==0);
	return c;
}

void main(void)
{
	char c;
	while(1)
	{
		c = _inchar();
		_outchar(c);
	}
}

