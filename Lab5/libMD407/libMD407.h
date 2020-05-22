/*
*	libNAME.h
*	Declaration of library functions, constants etc...
*/
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

/* Type definitions */
typedef struct
{
	char name[16];
	int (*init) (int);
	void (*deinit) (int);
	int (*fstat)(struct stat *st);
	int (*isatty)(void);
	int (*open)(const char name,int flags,int mode);
	int (*close)(void);
	int (*lseek)(int ptr, int dir);
	int (*write)(char *ptr, int len);
	int (*read)(char *ptr, int len);
} DEV_DRIVER_DESC, *PDEV_DRIVER_DESC;

/* Constants */
#define MAX_FILENO 5

/* Constant defined by linker */
extern char __heap_low;
extern char __heap_top;
extern char __bss_start;
extern char __bss_end;

/* Library defined functions */
void crt_init(void);
void crt_deinit(void);

char *_sbrk(int);
int _close(int file);
int _fstat(int file, struct stat *st);
int _isatty(int file);
int _lseek(int file, int ptr, int dir);
int _open(const char *name, int flags, int mode);
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);