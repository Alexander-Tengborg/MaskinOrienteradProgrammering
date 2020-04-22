/*
 * 	startup.c
 *
 */

#define		GPIO_E			0x40021000

#define 	GPIO_E_MODER 	((volatile int *) GPIO_E)
#define 	GPIO_E_OTYPER 	((volatile short *) GPIO_E+0x04)
#define 	GPIO_E_OSPEEDR 	((volatile int *) GPIO_E+0x08)
#define 	GPIO_E_PUPDR 	((volatile int *) GPIO_E+0x0C)

#define 	GPIO_E_IDR_LOW	((volatile char *) GPIO_E+0x10)
#define 	GPIO_E_IDR_HIGH ((volatile char *) GPIO_E+0x11)
#define 	GPIO_E_ODR_LOW	((volatile char *) GPIO_E+0x14)
#define 	GPIO_E_ODR_HIGH ((volatile char *) GPIO_E+0x15)

#define 	B_E 			0x40 /* Enable-signal */
#define 	B_SELECT 		4 /* Select ASCII-display */ 
#define 	B_RW 			2 /* 0=Write, 1=Read */ 
#define 	B_RS 			1 /* 0=Control, 1=Data */ 
 
#define 	SIMULATOR 

#define 	STK_CTRL 		((volatile unsigned int *) 0xE000E010)
#define 	STK_LOAD 		((volatile unsigned int *) 0xE000E014)
#define 	STK_VAL 		((volatile unsigned int *) 0xE000E018)
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

/*
 * Konfigurerar port E så att den kan användas som en utport
 */
void init_app()
{
	*GPIO_E_MODER = 0x55555555;
}

/*
 * Konfigurerar ascii-displayen med korrekt inställningar
 */
void ascii_init()
{
    ascii_ctrl_bit_clear(B_RS);
    ascii_ctrl_bit_clear(B_RW);
    ascii_command(0x38); // Function Set: 2 rader och 5x8 punkters tecken
    ascii_command(0xF); // Display Control: Tänd display, tänd markör (cursor) och konstant visning
    ascii_command(1); // Clear Display
    ascii_command(6); // Entry Mode Set: Adressing med "increment"; inget skift av adressbufferten
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
 * Sätter en bit till 1, med den givna bitmasken x
 */
void ascii_ctrl_bit_set(unsigned char x) {
	char c;
    c = *GPIO_E_ODR_LOW;
    *GPIO_E_ODR_LOW = B_SELECT | x | c;
}

/*
 * Sätter en bit till 0, med den givna bitmasken x
 */
void ascii_ctrl_bit_clear(unsigned char x) {
	char c;
	c = *GPIO_E_ODR_LOW;
	c = c & ~x;
	*GPIO_E_ODR_LOW = B_SELECT | c;
}
/*
 * Skriver data till ascii-displayens kontroller
 */
void ascii_write_controller(char c) {
	ascii_ctrl_bit_set(B_E);
	*GPIO_E_ODR_HIGH = c;
	ascii_ctrl_bit_clear(B_E);
	delay_250ns();
}

/*
 * Skriver ett kommando till ascii-displayens kontroller
 */
void ascii_write_cmd(unsigned char command)
{
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(command);
}

/*
 * Skriver data till ascii-displayen
 */
void ascii_write_data(unsigned char data)
{
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(data);
}

/*
 * Läser data från ascii-displayens kontroller
 */
unsigned char ascii_read_controller(void) {
	char c;
	ascii_ctrl_bit_set(B_E);
	delay_250ns();
	delay_250ns();
	c = *GPIO_E_IDR_HIGH;
	ascii_ctrl_bit_clear(B_E);
	return c;
}

/*
 * Läser ascii-displayens status
 */
unsigned char ascii_read_status() {
	char c;
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_set(B_RW);
	ascii_ctrl_bit_clear(B_RS);
	c = ascii_read_controller();
	*GPIO_E_MODER = 0x55555555;
	return c;
}

/*
 * Läser data från ascii-displayen
 */
unsigned char ascii_read_data(void)
{
	char c;
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	c = ascii_read_controller();
	*GPIO_E_MODER = 0x55555555;
	return c;
}

/*
 * Skickat ett kommando till ascii-displayen
 */
void ascii_command(unsigned char command)
{
	while((ascii_read_status() & 0x80) == 0x80);
	delay_micro(8);
	ascii_write_cmd(command);
	
	if((command == 1 || (command & 2) == 2)) 	delay_milli(2);
	else if((command & 0x40) == 1) return;
	else delay_micro(45);
}

/*
 * Skriver en given char c till ascii-displayen
 */
void ascii_write_char(unsigned char c)
{
	while((ascii_read_status() & 0x80) == 0x80);
	delay_micro(8);
	ascii_write_data(c);
	delay_micro(45);
}

/*
 * Ändrar på den aktiva raden och columnen
 */
void ascii_gotoxy(unsigned char row, unsigned char col)
{
	unsigned int address = row - 1;
	if(col == 2) address = address + 0x40;
	ascii_write_cmd(0x80 | address);
}

/* 
 * Används för att returna den knappen som är nertryckt
 */
void main(void)
{
	char *s;
	char test1[] = "Alfanumerisk ";
	char test2[] = "Display - test";
	init_app();
	ascii_init();
	ascii_gotoxy(1,1);
	s = test1;
	while(*s)
		ascii_write_char(*s++);
	ascii_gotoxy(1,2);
	s = test2;
	while(*s)
		ascii_write_char(*s++);
	return 0; 
}