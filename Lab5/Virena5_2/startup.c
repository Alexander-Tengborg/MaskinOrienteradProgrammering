/*
 * 	startup.c
 *
 */
#define MAX_POINTS				25
#define GPIO_D              	0x40020C00
#define GPIO_D_MODER        ((volatile unsigned int *) GPIO_D)
 
#define GPIO_D_OTYPER       ((volatile unsigned short *) GPIO_D+0x04)
#define GPIO_D_OTYPER_LOW   ((volatile unsigned char *) GPIO_D+0x04)
#define GPIO_D_OTYPER_HIGH  ((volatile unsigned char *) GPIO_D+0x05)
 
#define GPIO_D_PUPDR          ((volatile unsigned long *) GPIO_D+0x0C)
#define GPIO_D_PUPDR_LOW      ((volatile unsigned short *) GPIO_D+0x0C)
#define GPIO_D_PUPDR_HIGH     ((volatile unsigned short *) GPIO_D+0x0E)
 
#define GPIO_D_IDR          ((volatile unsigned short *) GPIO_D+0x10)
#define GPIO_D_IDR_LOW      ((volatile unsigned char *) GPIO_D+0x10)
#define GPIO_D_IDR_HIGH     ((volatile unsigned char *) GPIO_D+0x11)
 
#define GPIO_D_ODR          ((volatile unsigned short *) GPIO_D+0x14)
#define GPIO_D_ODR_LOW      ((volatile unsigned char *) GPIO_D+0x14)
#define GPIO_D_ODR_HIGH     ((volatile unsigned char *) GPIO_D+0x15)

#define SIMULATOR
#define STK_CTRL 			((volatile unsigned int *)(0xE000E010))
#define STK_LOAD 			((volatile unsigned int *)(0xE000E014))
#define STK_VAL 			((volatile unsigned int *)(0xE000E018))
 
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
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

void init_app(void)
{
    // Keypad setup
    *GPIO_D_MODER &= 0x0000FFFF;
    *GPIO_D_MODER = 0x55000000;
    *GPIO_D_OTYPER_HIGH = 0x00;
    *GPIO_D_PUPDR_HIGH = 0x00AA;
 
}

void activateRow(int row)
{
    *GPIO_D_ODR_HIGH = 0x10 << (row-1);
}
 
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
 
unsigned char keyb(void) {
   unsigned char key[]={1,2,3,0xA,4,5,6,0xB,7,8,9,0xC,0xE,0,0xF,0xD};
                           
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

void delay_250ns( void )
	{
	*STK_CTRL = 0;
	*STK_LOAD = ( (168/4) -1 );
	*STK_VAL = 0;
	*STK_CTRL = 5; 

	while((*STK_CTRL & 0x10000) == 0 );
	*STK_CTRL = 0;
	}

void delay_milli (unsigned int ms) 
{
	delay_micro(ms*1000);
}

void delay_micro (unsigned int us)
{
	
	#ifdef SIMULATOR
		us = us / 8000;
		us++;
	#endif

	while( us > 0 )
	{
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
		us--;
	}
}


typedef struct
{
  int x,y;
} POINT, *PPOINT;

typedef struct
{
   POINT P0;
   POINT P1;
} LINE, *PLINE;

typedef struct
{
  POINT P_start;
  int x,y;
} RECT,*PRECT;

typedef struct polygonpoint
{
  char x,y;
  struct polygonpoint *next;
} POLY, *PPOLY;


typedef struct
{
  int numpoints;
  int sizex;
  int sizey;
  POINT px[ MAX_POINTS ];
} GEOMETRY, *PGEOMETRY;

typedef struct tObj{
	PGEOMETRY geo;
	int dirx,diry;
	int posx,posy;
	void (* draw ) (struct tObj *);
	void (* clear ) (struct tObj *);
	void (* move ) (struct tObj *);
	void (* set_speed ) (struct tObj *, int, int);
} OBJECT, *POBJECT; 

void swap(int *x, int *y) 
{ 
    int temp = *x; 
    *x = *y; 
    *y = temp; 
} 

int draw_rect(RECT r ){
	POINT P_start = r.P_start;
	POINT P_end = {r.P_start.x+r.x, r.P_start.y};
	LINE line = {P_start, P_end};
	draw_line(line);
	
	P_start = P_end;
	P_end.y += r.y;
	line.P0 = P_start;
	line.P1 = P_end;
	draw_line(line);
	
	P_start=P_end;
	P_end.x -= r.x;
	line.P0 = P_start;
	line.P1 = P_end;
	draw_line(line);
	
	P_start = P_end;
	P_end.y -= r.y;
	line.P0 = P_start;
	line.P1 = P_end;
	draw_line(line);
}

int draw_polygon(PPOLY poly){
	POINT p0 = {poly->x,poly->y};
	PPOLY ptr = poly->next;
	while(ptr!=0){
		POINT p1={ptr->x, ptr->y};
		LINE line ={p0,p1};
		draw_line(line);
		p0=p1;
		ptr=ptr->next;
	}
}

int draw_line( LINE l )
{
	
	// Ifall punkten är utanför skärmen så hantera det. Gör en if-sats
	
	int steep = abs(l.P1.y-l.P0.y) > abs(l.P1.x-l.P0.x);
	if(steep){
		swap(&l.P0.x, &l.P0.y);
		swap(&l.P1.x, &l.P1.y);
	} 
	
	if(l.P0.x > l.P1.x){
		swap(&l.P0.x, &l.P1.x);
		swap(&l.P0.y, &l.P1.y);
	}
	
	int deltax = (l.P1.x-l.P0.x);
	int deltay = abs(l.P1.y-l.P0.y);
	int error = 0; 
	int ystep; 
	int y = l.P0.y;
	
	if(l.P0.y<l.P1.y){
		ystep = 1;
		}
	else 
	{
		ystep = -1;
	}
	
	for(int x = l.P0.x; x<= l.P1.x; x++){
		if(steep){
			graphic_pixel_set(y,x);
		}
		else{
			graphic_pixel_set(x,y);
		}
		error = error + deltay;
		if((2*error)>=deltax){
			y=y+ystep;
			error = error - deltax;
			}
		}
}
	
	GEOMETRY spider_geometry[] = {
		  22, /* numpoints */
		6,8, /* sizex,sizey */
    {
        /* px[0,1,2 ...] */
        {2,0},{3,0},{1,1},{4,1},{0,2},{1,2},{2,2},{3,2},{4,2},{5,2},{0,3},
        {2,3},{3,3},{5,3},{1,4},{4,4},{2,5},{3,5},{1,6},{4,6},{0,7},{5,7}
    }
	}; 
	

GEOMETRY ball_geometry =
{ 
	16, 7, 7,
 { {0,2}, {0,3}, {0,4}, {1,1}, {1,5},
	{2,0}, {2,6}, {3,0}, {3,6}, {4,0}, {4,6},
	{5,1}, {5,5}, {6,2}, {6,3}, {6,4}       	 	
 
 }
};
	/*{
		24, // numpoints
		4,4, // sizex,sizey
	{
		// px[0,1,2 ...]
		{0,1},{0,2},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{3,1}, {3,2}
	}
	}; 
*/
void draw_object(POBJECT object){
	
	char numpoints =object->geo->numpoints;
	for(short i=0; i<numpoints; i++) {
		short a = object->geo->px[i].x+object->posx;
		short b = object->geo->px[i].y+object->posy;
		graphic_pixel_set(a,b);
	}
}

void clear_object(POBJECT object){
		char numpoints =object->geo->numpoints;
		for(short i=0; i<numpoints; i++) {
		short a = object->geo->px[i].x+object->posx;
		short b = object->geo->px[i].y+object->posy;
		graphic_pixel_clear(a,b);
	}	
}

void move_spiderobject(POBJECT object){
	clear_object(object);
	int newX = object ->posx+ object->dirx;
	int newY = object ->posy+ object->diry;
	
	object->posx=newX;
	object->posy=newY;
	
	if (newX<1){
		object->dirx = -object->dirx;
	}
	
	if((newX+object->geo->sizex)>128){
		object->dirx = -object->dirx;
	}
	if (newY<1){
		object->diry = -object->diry;
	}
	
	if((newY+object->geo->sizey)>64){
		object->diry = -object->diry;
	}
	
	draw_object(object);
}

void move_object(POBJECT object){
	clear_object(object);
    short newX = object->posx+object->dirx;
    short newY = object->posy+object->diry;
   
    object->posx = newX;
    object->posy = newY;
   
    if(newX<1) {
        object->posx = 128;
    }


    draw_object(object);
}
void set_object_speed(POBJECT object, int xSpeed, int ySpeed){
	object->dirx = xSpeed;
	object->diry = ySpeed;
}
static OBJECT spider = 
{
	&spider_geometry, /* geometri för en boll */
	0,0, /* initiala riktningskoordinater */
	5,5, /* initial startposition */
	draw_object,
	clear_object,
	move_spiderobject,   
	set_object_speed
};

static OBJECT ball =
{
	&ball_geometry, /* geometri för en boll */
	0,0,
	121,55,
	draw_object,
	clear_object,
	move_object,
	set_object_speed
};

int objects_overlap(POBJECT ball, POBJECT spider)
{
    if((ball->posx + ball->geo->sizex) >= spider->posx && ball->posx <= (spider->posx + spider->geo->sizex)
        && (ball->posy + ball->geo->sizey) >= spider->posy && ball->posy <= (spider->posy + spider->geo->sizey))
        {
            return 1;
        }
    return 0;
}

/*int is_outside_x(POBJECT object)
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

 */

void main(void)
{
    char c;
    POBJECT victim = &ball;
    POBJECT creature = &spider;
    init_app();   
    graphic_initalize();
    graphic_clear_screen();
    victim->set_speed( victim, -3, 0);
       
    POINT p0 = {0, 63};
    POINT p1 = {128, 63};
    LINE l = {p0, p1};

    draw_line(l);
       
    while( 1 )
    {
        victim->move(victim);
        creature->move(creature);
        c = keyb();
        
        switch(c)
        {
        case 6: creature->set_speed( creature, 2, 0); break;   
        case 4: creature->set_speed( creature, -2, 0); break;   
        case 5: creature->set_speed( creature, 0, 0); break;   
        case 2: creature->set_speed( creature, 0, -2); break;   
        case 8: creature->set_speed( creature, 0, 2); break;
                                                        default:
                    creature->set_speed( creature, 0, 0); break;
        }
             if(objects_overlap(victim, creature) /*|| is_outside(creature)*/)
             {
                // Game over
                break;
             }
         delay_milli(20);
         
    }
}