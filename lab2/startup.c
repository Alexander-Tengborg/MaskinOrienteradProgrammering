/*
 * 	startup.c
 *
 */
 

/* Konstantdefinitioner (portadresser) */
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup(void)
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

void app_init(void)
{
    * ((unsigned short *) 0x40020C02) = 0x5500; //GPIO_MODER
    * ((unsigned char *) 0x40020C05) = 0x0000; //GPIO_OTYPER
    * ((unsigned short *) 0x40020C0E) = 0x00AA; //GIP_PUPDR
}

/* Hjälprutiner */

void activateRow(void)
{
    
}

void readColumn(void)
{
    
}

unsigned char keyb(void)
{

}

void out7seg(unsigned char c)
{
    
}

void main(void)
{
    unsigned char c;
    app_init();
    while(1) {
        c = keyb();
        out7seg(c);
    }
}

