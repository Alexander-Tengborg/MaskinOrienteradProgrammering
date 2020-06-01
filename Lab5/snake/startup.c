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
void graphic_pixel_set( int x, int y)
{
__asm volatile (" .HWORD 0xDFF2\n");
__asm volatile (" BX LR\n");
}

__attribute__((naked))
void graphic_pixel_clear( int x, int y)
{
__asm volatile (" .HWORD 0xDFF3\n");
__asm volatile (" BX LR\n"); 
}

#define MAX_BLOCKS 40
#define BLOCK_SIZE 4
#define SNAKE_SPEED 4 // SNAKE_SPEED = BLOCK_SIZE
//128 % BLOCK_SIZE och 128 % SNAKE_SPEED bör = 0 

#define     SIMULATOR
 
#define     STK_CTRL        ((volatile unsigned int *) 0xE000E010)
#define     STK_LOAD        ((volatile unsigned int *) 0xE000E014)
#define     STK_VAL         ((volatile unsigned int *) 0xE000E018)

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

typedef struct
{
	short x,y;
} POINT, *PPOINT;

typedef struct
{
	POINT pos;
	char intialized;
	POINT px[BLOCK_SIZE*BLOCK_SIZE];
	//Method for creating new blocks
} BLOCK, *PBLOCK;

typedef struct tSnake
{
	BLOCK blocks[MAX_BLOCKS];
	signed int dirx,diry;
	POINT headPos;
	POINT tailPos;
	int numberOfBlocks;
	void (*draw) (struct tSnake *);
	void (*clear) (struct tSnake *);
	void (*move) (struct tSnake *);
	void (*set_speed) (struct tSnake *, int, int);
} SNAKE, *PSNAKE; 

BLOCK create_block(POINT pos)
{
	BLOCK block;
	block.pos = pos;
	block.intialized = 1;
	
	for(int i = 0; i < BLOCK_SIZE; i++)
	{
		for(int j = 0; j < BLOCK_SIZE; j++)
		{
			POINT px = {i, j};
			//printf("%d %d\n",i, j);
			block.px[i*BLOCK_SIZE + j] = px;
		}
	}
	return block;
}

void draw_block(BLOCK block)
{	
	for(int i = 0; i < BLOCK_SIZE; i++)
	{
		for(int j = 0; j < BLOCK_SIZE; j++)
		{
			POINT px = block.px[i*BLOCK_SIZE + j];
			graphic_pixel_set(px.x+block.pos.x, px.y+block.pos.y);
		}
	}
}

void clear_block(BLOCK block)
{	
	for(int i = 0; i < BLOCK_SIZE; i++)
	{
		for(int j = 0; j < BLOCK_SIZE; j++)
		{
			POINT px = block.px[i*BLOCK_SIZE + j];
			graphic_pixel_clear(px.x+block.pos.x, px.y+block.pos.y);
		}
	}
}

//Ritar alla ormen block, som dock inte behövs (behöver bara rita
//huvudet om man hela tiden ökar antalet block med 1)
/*void draw_snake(PSNAKE snake)
{
	//printf("%d",snake->numberOfBlocks);
	for(int i = 0; i < snake->numberOfBlocks; i++)
	{
		
		if(!snake->blocks[i].intialized)
		{
			snake->blocks[i] = create_block(snake->tailPos);
		}
		
		draw_block(snake->blocks[i]);
	}
}*/

//Ritar ett block - Byt namn till draw_head?
void draw_snake(PSNAKE snake)
{
	if(!snake->blocks[snake->numberOfBlocks-1].intialized)
	{
		snake->blocks[snake->numberOfBlocks-1] = create_block(snake->tailPos);
	}
	
	draw_block(snake->blocks[0]);
}

//Tar bort alla ormen block, som dock inte behövs (behöver bara ta bort
//rumpan om man hela tiden ökar antalet block med 1)
/*void clear_snake(PSNAKE snake)
{
	for(int i = 0; i < snake->numberOfBlocks; i++)
	{		
		clear_block(snake->blocks[i]);
	}
}*/

//Tar bort ett block - Byt namn till clear_tail?
void clear_snake(PSNAKE snake)
{
	clear_block(snake->blocks[snake->numberOfBlocks-1]);
}

void move_snake(PSNAKE snake)
{
	snake->clear(snake); //Man behöver inte ta bort hela ormen, räcker att ta bort "svansen" dvs den sista
	
	if(snake->blocks[snake->numberOfBlocks-1].intialized)
	{
		snake->tailPos.x = snake->blocks[snake->numberOfBlocks-1].pos.x;
		snake->tailPos.y = snake->blocks[snake->numberOfBlocks-1].pos.y;	
	}
	else
	{
		snake->tailPos.x = snake->blocks[snake->numberOfBlocks-2].pos.x;
		snake->tailPos.y = snake->blocks[snake->numberOfBlocks-2].pos.y;	
	}
	
	for(int i = snake->numberOfBlocks - 1; i > 0; i--)
	{
		snake->blocks[i].pos = snake->blocks[i-1].pos;
	}
	
	snake->blocks[0].pos.x += snake->dirx;
	snake->blocks[0].pos.y += snake->diry;
	
	snake->headPos.x += snake->dirx;
	snake->headPos.y += snake->diry;
	
	snake->draw(snake);  //På samma sätt som clear så behöver man inte måla hela ormen, räcker att måla huvudet, dvs den första
}

void set_snake_speed(PSNAKE snake, short xSpeed, short ySpeed)
{
	if((-xSpeed) == snake->dirx || (-ySpeed) == snake->diry) return;
	snake->dirx = xSpeed;
	snake->diry = ySpeed;
}

void add_block(PSNAKE snake)
{
	if(snake->numberOfBlocks != (MAX_BLOCKS - 1))
		snake->numberOfBlocks++;
}

SNAKE snakeObj =
{
	{0}, /* BLOCK[] (ormens olika block) */
	SNAKE_SPEED,0, /* initiala riktningskoordinater */
	52,30, /* Huvudets position */ 
	52,30, /* Svansens position */ //fixa konstant ^
	1, /* Antal block */
	draw_snake,
	clear_snake,
	move_snake,
	set_snake_speed
}; 

//Ormen har ingen kollision
//Dvs gameover när ormen åker ut från banan eller in i sig själv

//"Spawna" ett block med random x och y koordinater som inte ligger på ormen
//Fixa kollision med ormens huvud och detta block...

//Skriv ut t.ex "Tryck på 7 för att starta!" och implementera....
//Dvs while ( c = read(KEYPAD, buffer, 1 ) == 0); 

void main(void)
{
	char c;
	PSNAKE snake = &snakeObj;
	
	printf("STARTED\n");
	printf("%d\n\n", snake->numberOfBlocks);
	
	graphic_initalize();
	graphic_clear_screen();
	
	
	snake->draw(snake);
	
	while(1)
	{
		
		snake->move(snake);
		c = _read(KEYPAD,0,1);
		//printf("%d\n",c);
		switch(c)
        {
            case 6: snake->set_speed(snake, SNAKE_SPEED, 0); break;
            case 4: snake->set_speed(snake, -SNAKE_SPEED, 0); break;
            case 5: snake->set_speed(snake, 0, 0); break; //FOR DEBUGGING PURPOSES
            case 2: snake->set_speed(snake, 0, -SNAKE_SPEED); break;
            case 8: snake->set_speed(snake, 0, SNAKE_SPEED); break;
			case 7: add_block(snake); break;
        }
		delay_milli(100);
	}
}

