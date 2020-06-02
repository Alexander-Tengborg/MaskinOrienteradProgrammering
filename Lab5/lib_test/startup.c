/*
 * 	startup.c
 *
 */
#include <stdio.h>
#include <libMD407.h>

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup (void)
{
__asm__ volatile(" LDR R0,=__stack_top\n");
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL _crt_init\n");
__asm__ volatile(" BL main\n");
__asm__ volatile(" BL _crt_deinit\n");
__asm__ volatile("_exit: B _exit\n");
__asm__ volatile(" .globl _exit\n");
}

void main(void)
{
	//Läsning från KEYPAD
	char c;
	while(1)
	{
		c = _read(KEYPAD,0,1);
		printf("%d\n",c);
	}
	// KEYPAD READ TEST END

	//Läsning/Skrivning till USART
	/*char c;
	while(1)
	{
		c = getc(stdin);
		putchar(c);
	}*/

	//Malloc och Free
	int *p;

	p = (int *)malloc(sizeof(int));
	if (p == 0)
	{
		printf("ERROR: Out of memory\n");
		return 1;
	}
	*p = 5;
	printf("%d\n", *p);
	free(p);


	//Skriva till ASCII 
	dprintf(ASCIIDISPLAY,"Hello World");


	//Skriva till USART
	printf("Hello World\n");
		
}

