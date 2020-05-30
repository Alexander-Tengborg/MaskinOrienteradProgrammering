/*
 * libMD407
 * keypad_driver.c
 * Keypad connected to PD8-PD15
 * Driver for KEYPAD
 */
#include "libMD407.h"

static int keypad_init(int initval);
static void keypad_deinit(int deinitval);
static int keypad_read(char *ptr, int len);

DEV_DRIVER_DESC KeyPad =
{
	{"Keypad"},
	keypad_init,
	keypad_deinit,
	0,
	0,
	0,
	0,
	0,
	0,
	keypad_read
};

/* KEYPAD types and constants definitions */

#define     GPIO_D              0x40020C00
#define     GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)
#define     GPIO_D_OTYPER_HIGH  ((volatile unsigned char *) GPIO_D+0x05)
#define     GPIO_D_PUPDR_HIGH   ((volatile unsigned short *) GPIO_D+0x0E)
#define     GPIO_D_IDR_HIGH     ((volatile unsigned char *) GPIO_D+0x11)
#define     GPIO_D_ODR_HIGH     ((volatile unsigned char *) GPIO_D+0x15)

/* Define functions here */
static int keypad_init(int initval)
{
	*GPIO_D_MODER &= 0x0000FFFF;
    *GPIO_D_MODER |= 0x55000000;
    *GPIO_D_OTYPER_HIGH = 0x00;
    *GPIO_D_PUPDR_HIGH = 0x00AA;
}

static void keypad_deinit(int deinitval)
{
	
}

static void keypad_activateRow(int row)
{
    *GPIO_D_ODR_HIGH = 0x10 << (row-1);
}

static unsigned char keypad_getColumn(void)
{
    unsigned char c;
    c = *GPIO_D_IDR_HIGH;
    if (c & 0x8) return 4;
    if (c & 0x4) return 3;
    if (c & 0x2) return 2;
    if (c & 0x1) return 1;
    return 0;
}

static int keypad_read(char *ptr, int len)
{
	unsigned char key[] = {1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};						
	for (unsigned char row = 1; row <= 4; row++) {
        keypad_activateRow(row);
        unsigned char col = keypad_getColumn();
        if (col != 0) {
            keypad_activateRow(0);
            return key[4*(row-1)+(col-1)];
        }
    }
    keypad_activateRow(0);
    return 0xFF;
}