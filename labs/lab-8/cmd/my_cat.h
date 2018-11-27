/************* myls.c file **********/

#ifndef __MYCAT__
#define __MYCAT__

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

int my_cat(int myargc, char *myargv[]);

#endif