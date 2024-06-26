/*
 * 	startup.c
 *
 */
 
#define     GPIO_D              0x40020C00
#define     GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)
#define     GPIO_D_ODR_LOW     ((volatile unsigned char *) GPIO_D+0x14)

#define     STK_CTRL        ((volatile unsigned int *) 0xE000E010)
#define     STK_LOAD        ((volatile unsigned int *) 0xE000E014)
#define     STK_VAL         ((volatile unsigned int *) 0xE000E018)

#define		VECTOR_DEST		0x2001C000
#define		SCB_VTOR		((volatile unsigned int *) 0xE000ED08)

#define SIMULATOR

#ifdef  SIMULATOR  
#define DELAY_COUNT 100
#else
#define DELAY_COUNT 1000000
#endif

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

volatile unsigned int delay_count;
volatile unsigned int systick_flag;


void delay_1mikro(void)
{
    systick_flag = 0;
    *STK_CTRL = 0;
    *STK_LOAD = (168 - 1);
    *STK_VAL = 0;
    *STK_CTRL = 7;
}

void delay(unsigned int count)
{
    delay_count = count;
    delay_1mikro();
}

void systick_irq_handler(void)
{
	*STK_CTRL = 0;
    delay_count = delay_count -1;
    if(delay_count > 0)
        delay_1mikro();
    else
        systick_flag = 1;
}

void init_app(void)
{
    /* Initiera port D och undantagsvektor */
	*GPIO_D_MODER = 0x00005555;
	
	*SCB_VTOR = VECTOR_DEST;
	
	*((void (**)(void)) (VECTOR_DEST+0x3C)) = systick_irq_handler;
}

void main(void)
{
    init_app();
    *GPIO_D_ODR_LOW = 0;
    delay(DELAY_COUNT);
    *GPIO_D_ODR_LOW = 0xFF;
    while(1)
    {
        if(systick_flag)
            break;
    }
    *GPIO_D_ODR_LOW = 0;
}

