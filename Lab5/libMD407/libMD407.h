/*
* libMD407.h
* Typer och konstanter som används i hela programbiblioteket
*/
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct
{
	char name[16];
	int (*init) (int);
	void (*deinit) (int);
	int (*fstat) (struct stat *st);
	int (*isatty) (int);
	int (*open) (const char name, int flags, int mode);
	int (*close )(void);
	int (*lseek) (int ptr, int dir);
	int (*write) (char *ptr, int len);
	int (*read) (char *ptr, int len);
} DEV_DRIVER_DESC, *PDEV_DRIVER_DESC;

/* File handle constants */

enum
{
	STDIN = 0,
	STDOUT,
	STDERR,
	KEYPAD,
	ASCIIDISPLAY
};

#define MAX_FILENO ASCIIDISPLAY

/* Library defined functions */
int _close(int file);
int _open(const char *name, int flags, int mode);
int _fstat(int file, struct stat *st);
int _lseek(int file, int ptr, int dir);
int _isatty(int file);
int _read(int file, char *ptr, int len);
int _write(int file, char *ptr, int len);