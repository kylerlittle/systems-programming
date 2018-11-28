#ifndef __MYRMDIR__
#define __MYRMDIR__

#include "../util/globals.h"

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[CHUNK_SIZE];

int my_rmdir(int myargc, char *myargv[]);

#endif
