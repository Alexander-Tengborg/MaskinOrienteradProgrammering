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

unsigned long *gpiod = 0x40020C00;
volatile char *tgp = 0x40020C02;
void app_init(void)
{
    //*gpiod = 0x55550000;
    unsigned short c1 = 0x5555;
    //* ((unsigned short*) 0x40020C02) = c1;
    * ((volatile unsigned char*) 0x40020C02) = 0x55;
    * ((unsigned long*) 0x40020C14) = 0xFFFFFFFF;
    //*tgp=0x55;
}

void main(void)
{
    unsigned short c1;
    unsigned short c2;
    app_init();
    while(1) {
        c1 = 'A';
        c2 = 4;
        * ((unsigned short*) 0x40020C14) = c1<<c2;

    }
}

