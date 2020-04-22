/*
 *  startup.c
 *
 */
 
 
#define     GPIO_E          0x40021000
#define     GPIO_E_MODER    ((volatile unsigned int *)  GPIO_E)
#define     GPIO_E_ODR_LOW  ((volatile unsigned char *) GPIO_E+0x14)
 
#define     SIMULATOR
 
#define     STK_CTRL        ((volatile unsigned int *) 0xE000E010)
#define     STK_LOAD        ((volatile unsigned int *) 0xE000E014)
#define     STK_VAL         ((volatile unsigned int *) 0xE000E018)
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup(void)
{
__asm__ volatile(" LDR R0,=0x2001C000\n");      /* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");                 /* call main */
__asm__ volatile(".L1: B .L1\n");               /* never return */
}

/*
 * Konfigurerar port E så att bitar 0-7 kan användas som en utport
 */
void init_app(void)
{
    *GPIO_E_MODER &= 0xFFFF0000;
    *GPIO_E_MODER |= 0x00005555;
}
 
/*
 * Funktion som gör så att simulatorn "sover" i 250 nanosekunder
 */
void delay_250ns(void)
{
	*STK_CTRL = 0;
	*STK_LOAD = ( (168/4) -1 );
	*STK_VAL = 0;
	*STK_CTRL = 5;
	while( (*STK_CTRL & 0x10000 )== 0 );
	*STK_CTRL = 0;
}

/*
 * Funktion som gör så att simulatorn "sover" i us antal mikrosekunder
 */
void delay_micro(unsigned int us)
{
	#ifdef SIMULATOR
		us = us / 2000;
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

/*
 * Funktion som gör så att simulatorn "sover" i ms antal millisekunder
 */
void delay_milli(unsigned int ms)
{
	delay_micro(ms*1000);
}

/* 
 * Används för att returna den knappen som är nertryckt
 */
void main(void)
{
    init_app();
    while(1)
    {
        *GPIO_E_ODR_LOW = 0;
        delay_milli(500);
        *GPIO_E_ODR_LOW = 0xFF;
        delay_milli(500);
    }
}