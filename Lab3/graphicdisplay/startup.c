/*
 * 	startup.c
 *
 */
 
#define 	SIMULATOR 

#define 	STK_CTRL 		((volatile unsigned int *) 0xE000E010)
#define 	STK_LOAD 		((volatile unsigned int *) 0xE000E014)
#define 	STK_VAL 		((volatile unsigned int *) 0xE000E018)
 
void startup(void);

void graphic_initalize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

void delay_250ns(void);
void delay_micro(unsigned int us);
void delay_milli(unsigned int ms);

void main(void)
{
    int i;
	graphic_initalize();
	graphic_clear_screen();
	for(i = 1; i <= 128; i++) /* rita en horisonell linje */
        graphic_pixel_set(i, 10);
    for(i = 1; i <= 64; i++) /* rita en vertikal linje */
        graphic_pixel_set(10, i);
    delay_milli(5000); /* vänta 0,5 sekunder */
	for(i = 1; i <= 128; i++)
        graphic_pixel_clear(i, 10); /* sudda horisontella linjen */
    for(i = 1; i <= 64; i++)
        graphic_pixel_clear(10, i); /* sudda vertikala linjen */
} 

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

__attribute__((naked))
void graphic_initalize(void)
{
__asm volatile (" .HWORD 0xDFF0\n");
__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_clear_screen(void)
{
__asm volatile (" .HWORD 0xDFF1\n");
__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_set( int x, int y)
{
__asm volatile (" .HWORD 0xDFF2\n");
__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_clear( int x, int y)
{
__asm volatile (" .HWORD 0xDFF3\n");
__asm volatile (" BX LR\n"); 
}

void delay_250ns(void)
{
	/* SystemCoreClock = 168000000 */
	*STK_CTRL = 0;
	*STK_LOAD = ( (168/4) -1 );
	*STK_VAL = 0;
	*STK_CTRL = 5;
	while( (*STK_CTRL & 0x10000 )== 0 );
	*STK_CTRL = 0;
}

void delay_micro(unsigned int us)
{
	#ifdef SIMULATOR
		us = us / 2500;
		us++;
	#endif
	
	while(us > 0)
	{
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
		us--;
	}
}

void delay_milli(unsigned int ms)
{
	delay_micro(ms*1000);
}