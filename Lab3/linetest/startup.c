/*
 * 	startup.c
 *
 */
 
#define 	SIMULATOR 

#define 	STK_CTRL 		((volatile unsigned int *) 0xE000E010)
#define 	STK_LOAD 		((volatile unsigned int *) 0xE000E014)
#define 	STK_VAL 		((volatile unsigned int *) 0xE000E018)

typedef struct
{
	short x,y;
} POINT, *PPOINT;

typedef struct
{
   POINT p0;
   POINT p1;
} LINE, *PLINE;

void startup(void);

void graphic_initalize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

void delay_250ns(void);
void delay_micro(unsigned int us);
void delay_milli(unsigned int ms);

void swap(short *x, short *y);

int can_plot_line(LINE l);
int draw_line(LINE l);

LINE lines[]={
	{40,10, 100,10},
	{40,10, 100,20},
	{40,10, 100,30},
	{40,10, 100,40},
	{40,10, 100,50},
	{40,10, 100,60},
	{40,10, 90,60},
	{40,10, 80,60},
	{40,10, 70,60},
	{40,10, 60,60},
	{40,10, 50,60},
	{40,10, 40,60}
};

void main(void)
{
	graphic_initalize();
	graphic_clear_screen();
  
	while( 1 )
	{
		for( int i = 0; i< sizeof(lines)/sizeof(LINE); i++)
		{
			draw_line(lines[i]);
			delay_milli(500);
		}
		graphic_clear_screen();
	}
}

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup(void)
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
void graphic_pixel_set(int x, int y)
{
__asm volatile (" .HWORD 0xDFF2\n");
__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_clear(int x, int y)
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

void swap(short *x, short *y)
{
    short temp = *x;
    *x = *y;
    *y = temp;
}

int can_plot_line(LINE l)
{
    POINT p0 = l.p0;
    POINT p1 = l.p1;
        
    if(p0.x < 0 || p0.x > 128 || p1.x < 0 || p1.x > 128) return 0; // Kolla x-koordinater
    if(p0.y < 0 || p0.y > 64 || p1.y < 0 || p1.y > 64) return 0; // Kolla y-koordinater
}

int draw_line(LINE l)
{	   
    if(!can_plot_line(l)) return 0;
    
	short steep = abs(l.p1.y - l.p0.y) > abs(l.p1.x - l.p0.x);
	if(steep) {
		swap(&l.p0.x, &l.p0.y);
		swap(&l.p1.x, &l.p1.y);
	}
	
	if(l.p0.x > l.p1.x) {
		swap(&l.p0.x, &l.p1.x);
		swap(&l.p0.y, &l.p1.y);
	}
	
	short deltax = l.p1.x - l.p0.x;
	short deltay = abs(l.p1.y - l.p0.y);
	short error = 0;
	short ystep = -1;
	short y = l.p0.y;	
	
	if(l.p0.y < l.p1.y) {
		ystep = 1;
	}
	
	for(short x = l.p0.x; x <= l.p1.x; x++) {
		if(steep) {
			graphic_pixel_set(y,x);
		} else {
			graphic_pixel_set(x,y);
		}
		error += deltay;
		if(2*error >= deltax) {
			y += ystep;
			error -=deltax;
		}
	}

	return 1;
}