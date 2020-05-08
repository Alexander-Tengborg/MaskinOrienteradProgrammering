/*
 *  startup.c
 *
 */
 
#define     GPIO_D              0x40020C00
#define     GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)
#define     GPIO_D_OTYPER_HIGH  ((volatile unsigned char *) GPIO_D+0x05)
#define     GPIO_D_PUPDR_HIGH   ((volatile unsigned short *) GPIO_D+0x0E)
#define     GPIO_D_IDR_HIGH     ((volatile unsigned char *) GPIO_D+0x11)
#define     GPIO_D_ODR_HIGH     ((volatile unsigned char *) GPIO_D+0x15)
 
#define     SIMULATOR
 
#define     STK_CTRL        ((volatile unsigned int *) 0xE000E010)
#define     STK_LOAD        ((volatile unsigned int *) 0xE000E014)
#define     STK_VAL         ((volatile unsigned int *) 0xE000E018)

typedef struct
{
	short x,y;
} POINT, *PPOINT;

#define MAX_POINTS 25
typedef struct
{
	int numpoints;
	int sizex;
	int sizey;
	POINT px[ MAX_POINTS ];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj {
	PGEOMETRY geo;
	signed int dirx,diry;
	int posx,posy;
	void (*draw) (struct tObj *);
	void (*clear) (struct tObj *);
	void (*move) (struct tObj *);
	void (*set_speed) (struct tObj *, int, int);
} OBJECT, *POBJECT; 

void startup(void);
void init_app(void);

void graphic_initalize(void);
void graphic_clear_screen(void);
void graphic_pixel_set(int x, int y);
void graphic_pixel_clear(int x, int y);

void delay_250ns(void);
void delay_micro(unsigned int us);
void delay_milli(unsigned int ms);

void activateRow(int row);
unsigned char getColumn(void);
unsigned char keyb(void);

void swap(short *x, short *y);

void draw_object(POBJECT object);
void clear_object(POBJECT object);
void move_object(POBJECT object);
void set_object_speed(POBJECT object, short xSpeed, short ySpeed);
int objects_overlap(POBJECT o1, POBJECT o2); 
int is_outside_x(POBJECT object);
int is_outside_y(POBJECT object);
int is_outside(POBJECT object);

GEOMETRY ball_geometry =
{
	12, /* numpoints */
	4,4, /* sizex,sizey */
	{
		/* px[0,1,2 ...] */
		{0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1},{3,2}
	}
}; 

GEOMETRY spider_geometry =
{
	22, /* numpoints */
	6,8, /* sizex,sizey */
	{
		/* px[0,1,2 ...] */
		{2,0},{3,0},{1,1},{4,1},{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{0,3},
        {2,3},{3,3},{5,3},{1,4},{4,4},{2,5},{3,5},{1,6},{4,6},{0,7},{5,7}
	}
}; 

static OBJECT ball =
{
	&ball_geometry, /* geometri för en boll */
	0,0, /* initiala riktningskoordinater */
	62,31, /* initial startposition */
	draw_object,
	clear_object,
	move_object,
	set_object_speed
}; 

static OBJECT spider =
{
	&spider_geometry, /* geometri för en boll */
	0,0, /* initiala riktningskoordinater */
	5,5, /* initial startposition */
	draw_object,
	clear_object,
	move_object,
	set_object_speed
}; 

int main(void)
{
    char c;
    POBJECT victim = &ball;
    POBJECT creature = &spider;
    init_app();
    graphic_initalize();
    graphic_clear_screen();
    //victim->set_speed(victim, 4, 1);

    while( 1 )
    {
        victim->move(victim);
        creature->move(creature);
        c = keyb();
        switch(c)
        {
            case 6: creature->set_speed(creature, 2, 0); break;
            case 4: creature->set_speed(creature, -2, 0); break;
            case 5: creature->set_speed(creature, 0, 0); break;
            case 2: creature->set_speed(creature, 0, -2); break;
            case 8: creature->set_speed(creature, 0, 2); break;
            default: creature->set_speed(creature, 0, 0); break;
        }
     
         if(objects_overlap(victim, creature) || is_outside(creature))
         {
            // Game over
            break;
         }
         
         delay_milli(20);
     }
}


__attribute__((naked)) __attribute__((section (".start_section")) )
void startup(void)
{
__asm__ volatile(" LDR R0,=0x2001C000\n");      /* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");                 /* call main */
__asm__ volatile(".L1: B .L1\n");                   /* never return */
}

void init_app(void)
{
    // Keypad setup
    *GPIO_D_MODER &= 0x0000FFFF;
    *GPIO_D_MODER = 0x55000000;
    *GPIO_D_OTYPER_HIGH = 0x00;
    *GPIO_D_PUPDR_HIGH = 0x00AA;
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
unsigned char keyb(void)
{
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

void draw_object(POBJECT object)
{
	char numpoints = object->geo->numpoints;
	//POINT points[] = object->geo->px;
	
	for(char c = 0; c < numpoints; c++)
	{
		graphic_pixel_set(object->posx+object->geo->px[c].x, object->posy+object->geo->px[c].y);
	}
}

void clear_object(POBJECT object)
{
	char numpoints = object->geo->numpoints;
	
	for(char c = 0; c < numpoints; c++)
	{
		graphic_pixel_clear(object->posx+object->geo->px[c].x, object->posy+object->geo->px[c].y);
	}
}

void move_object(POBJECT object)
{
	clear_object(object);
	short newX = object->posx+object->dirx;
	short newY = object->posy+object->diry;
	
	object->posx = newX;
	object->posy = newY;
    
	if(is_outside_x(object)) {
		object->dirx = -object->dirx;
	}

	if(is_outside_y(object)) {
		object->diry = -object->diry;
	}
	
	draw_object(object);
}

void set_object_speed(POBJECT object, short xSpeed, short ySpeed)
{
	object->dirx = xSpeed;
	object->diry = ySpeed;
}

int objects_overlap(POBJECT o1, POBJECT o2)
{
    if((o1->posx + o1->geo->sizex) >= o2->posx && o1->posx <= (o2->posx + o2->geo->sizex) 
        && (o1->posy + o1->geo->sizey) >= o2->posy && o1->posy <= (o2->posy + o2->geo->sizey))
        {
            return 1;
        }
    return 0;
}


int is_outside_x(POBJECT object)
{
    if(object->posx >= 1 && (object->posx + object->geo->sizex) <= 128) return 0;
}

int is_outside_y(POBJECT object)
{
	if(object->posy >= 1 && (object->posy + object->geo->sizey) <= 64) return 0;
}

int is_outside(POBJECT object)
{
    if(!is_outside_x(object) && !is_outside_y(object)) return 0;
}