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
	char c;
	while(1)
	{
		c = getc(KEYPAD);
		putc(c,stdout);
	}

	//GETC STDIN PUTCHAR STDOUT TEST
	/*char c;
	while(1)
	{
		c = getc(stdin);
		putchar(c);
	}*/
	//GETC STDIN PUTCHAR STDOUT TEST END
	
	//MALLOC & FREE TEST
	/*int *p;

	p = (int *)malloc(sizeof(int));
	if (p == 0)
	{
		printf("ERROR: Out of memory\n");
		return 1;
	}
	*p = 5;
	printf("%d\n", *p);
	free(p);
	//MALLOC & FREE TEST END

	dprintf(4,"Hello World\n"); //Skriva till ASCII //byt 4 till ASCIIDISPLAY
	
	printf("Hello World\n"); //Skriva till USART
	*/
	
	/*char str[20];
	
	printf("Enter name: ");
	//scanf("%s", str);
	gets(str);
	
	printf("Entered Name: %s\n", str);*/
	
	//Läsa från USART
	
		//Läsa från KEYPAD
	
	/*char buffer[256];
	printf("getline test");
	fflush(stdout);
	while(1)
	{
		getline(buffer, 256);
		fflush(stdin);
		
		printf("\n%s",buffer);
		fflush(stdout);
	}*/
}

