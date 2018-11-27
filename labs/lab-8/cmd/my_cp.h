#ifndef __MYCP__
#define __MYCP__

#include "../util/globals.h"

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[CHUNK_SIZE];

int my_cp(int myargc, char *myargv[]);

#endif
