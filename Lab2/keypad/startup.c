/*
 *  startup.c
 *
 */
 
/* Defintioner*/
#define     GPIO_D              0x40020C00
#define     GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)

#define     GPIO_D_OTYPER       ((volatile unsigned short *) GPIO_D+0x04)
#define     GPIO_D_OTYPER_LOW   ((volatile unsigned char *) GPIO_D+0x04)
#define     GPIO_D_OTYPER_HIGH  ((volatile unsigned char *) GPIO_D+0x05)

#define     GPIO_D_PUPDR        ((volatile unsigned long *) GPIO_D+0x0C)
#define     GPIO_D_PUPDR_LOW    ((volatile unsigned short *) GPIO_D+0x0C)
#define     GPIO_D_PUPDR_HIGH   ((volatile unsigned short *) GPIO_D+0x0E)

#define     GPIO_D_IDR          ((volatile unsigned short *) GPIO_D+0x10)
#define     GPIO_D_IDR_LOW      ((volatile unsigned char *) GPIO_D+0x10)
#define     GPIO_D_IDR_HIGH     ((volatile unsigned char *) GPIO_D+0x11)

#define     GPIO_D_ODR          ((volatile unsigned short *) GPIO_D+0x14)
#define     GPIO_D_ODR_LOW      ((volatile unsigned char *) GPIO_D+0x14)
#define     GPIO_D_ODR_HIGH     ((volatile unsigned char *) GPIO_D+0x15)
 
const unsigned char segs[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");      /* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");                 /* call main */
__asm__ volatile(".L1: B .L1\n");                   /* never return */
}

/*
 * Konfigurerar port D så att den kan användas med en keypad på bitar 0-7, 
 * samt så att en 7-segment display kan användas på bitar 8-15
 */
void app_init(void)
{
    // Keypad setup
    *GPIO_D_MODER &= 0x0000FFFF;
    *GPIO_D_MODER |= 0x55000000;
    *GPIO_D_OTYPER_HIGH = 0x00;
    *GPIO_D_PUPDR_HIGH = 0x00AA;
 
    // 7-seg setup
    *GPIO_D_MODER &= 0xFFFF0000;
    *GPIO_D_MODER |= 0x00005555;
}
 
/* 
 * Aktiverar rad row 
 */
void activateRow(int row)
{
    *GPIO_D_ODR_HIGH = 0x10 << (row-1);
}

/* 
 * Läser alla kolumner och returnerar ett värde beroende på vilken som är på
 */
unsigned char getColumn(void)
{
    unsigned char c;
    c = *GPIO_D_IDR_HIGH;
    if (c & 0x8) return 4;
    if (c & 0x4) return 3;
    if (c & 0x2) return 2;
    if (c & 0x1) return 1;
    return 0;
}
 
/* 
 * Används för att returna den knappen som är nertryckt
 */
unsigned char keyb(void) {
    unsigned char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};                           
    for (unsigned char row = 1; row <= 4; row++) {
        activateRow(row);
        unsigned char col = getColumn();
        if (col != 0) {
            activateRow(0);
            return key[4*(row-1)+(col-1)];
        }
    }
    activateRow(0);
    return 0xFF;
}

/* 
 * Ändrar värde på 7-segment displayen så att den visar den nertryckta knappen
 */
void out7seg(unsigned char c)
{
    if(c > 16) {
        *GPIO_D_ODR_LOW = 0;
        return;
    }
    *GPIO_D_ODR_LOW = segs[c];
}

/*
 * Programmets main funktion 
 */
void main(void)
{
    unsigned char c;
    app_init();
    while(1) {
        c = keyb();
        out7seg(c);
    }
}