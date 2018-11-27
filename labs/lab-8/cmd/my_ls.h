#ifndef __MYLS__
#define __MYLS__

#include "../util/globals.h"

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[CHUNK_SIZE];

int ls_file(char *fname);
int my_ls(int myargc, char *myargv[]);

#endif
