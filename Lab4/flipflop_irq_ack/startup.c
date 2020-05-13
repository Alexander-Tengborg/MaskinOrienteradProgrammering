/*
 * 	startup.c
 *
 */
 
#define     GPIO_D              0x40020C00
#define     GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)
#define     GPIO_D_ODR_LOW     	((volatile unsigned char *) GPIO_D+0x14)

#define     GPIO_E              0x40021000
#define     GPIO_E_MODER        ((volatile unsigned int *) GPIO_E)
#define     GPIO_E_ODR_LOW     	((volatile unsigned char *) GPIO_E+0x14)
#define 	GPIO_E_IDR_LOW		((volatile unsigned char *) GPIO_E+0x10)

#define		SYSCFG_EXTICR1		0x40013808

#define		EXTI_IMR			((volatile unsigned int*) 0x40013C00)
#define		EXTI_RTSR			((volatile unsigned int*) 0x40013C08)
#define		EXTI_PR				((volatile unsigned int*) 0x40013C14)
#define		EXTI_SWIER			((volatile unsigned int*) 0x40013C10)

#define		VECTOR_DEST		0x2001C000
#define		SCB_VTOR		((volatile unsigned int *) 0xE000ED08)

#define		NVIC_ISER0		((volatile unsigned int *) 0xE000E000)

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

volatile unsigned int count = 0; 

void interupt_handler(void)
{
	if(*EXTI_PR & (1<<3))
	{
		*EXTI_PR |= (1<<3);
		if(*GPIO_E_IDR_LOW & 1)
		{
			*GPIO_E_ODR_LOW |= (1 << 4);
			*GPIO_E_ODR_LOW &= ~(1 << 4);
			count++;
		}
		
		if(*GPIO_E_IDR_LOW & 2)
		{
			*GPIO_E_ODR_LOW |= (1 << 5);
			*GPIO_E_ODR_LOW &= ~(1 << 5);
			count = 0;
		}
		
		if(*GPIO_E_IDR_LOW && 4)
		{
			*GPIO_E_ODR_LOW |= (1 << 6);
			*GPIO_E_ODR_LOW &= ~(1 << 6);
			if(*GPIO_D_ODR_LOW != 0xFF)
			{
				*GPIO_D_ODR_LOW = 0xFF;
			} else {
				*GPIO_D_ODR_LOW = 0;
			}
		} else {
			*GPIO_D_ODR_LOW = count;
		}
	}
}

void init_app(void)
{
	*GPIO_D_MODER = 0x00005555;
	*GPIO_E_MODER = 0x00005500;
	
	//* Koppla PE3 till avbrottslina EXTI3 */
	*((unsigned int *) SYSCFG_EXTICR1) &= ~0xF000;
	*((unsigned int *) SYSCFG_EXTICR1) |= 0x4000;
	
	/* Konfigurera EXTI3 för att generera avbrott */
	*EXTI_IMR |= 8;
	
	/* Konfigurera för avbrott på positiv flank */
	*EXTI_RTSR |= 8;
	
	/* Sätt upp avbrottsvektor */
	*SCB_VTOR = VECTOR_DEST;
	*((void (**)(void)) (VECTOR_DEST+0x64)) = interupt_handler;
	
	/* Konfigurera de bitar i NVIC som kontrollerar den avbrottslina som EXTI3 kopplas til */
	//*NVIC_ISER0 |= (1<<9);	// IRQ NUM 9
	*((volatile unsigned int *) 0xE000E100) |= (1<<9);	// IRQ NUM 9
}

void main(void)
{
	init_app();
	while(1)
	{

	}
}

