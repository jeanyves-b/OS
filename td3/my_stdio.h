#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

typedef struct
{
	char *filename;
	int mode, fd;
}MY_FILE_s, *MY_FILE;
#define EOF -1

MY_FILE *my_fopen(char *name, char *mode);
int my_fclose(MY_FILE *f);
int my_fread(void *p, size_t taille, size_t nbelem, MY_FILE *f);
int my_fwrite(void *p, size_t taille, size_t nbelem, MY_FILE *f);
int my_feof(MY_FILE *f);

#include <mystdio.c>