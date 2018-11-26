/************* myls.c file **********/

#ifndef __MYLS__
#define __MYLS__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[4096];

int ls_file(char *fname);
int my_ls(int myargc, char *myargv[]);

#endif
