#ifndef __MYMKDIR__
#define __MYMKDIR__

#include "../util/globals.h"

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[CHUNK_SIZE];

int my_mkdir(int myargc, char *myargv[]);

#endif
