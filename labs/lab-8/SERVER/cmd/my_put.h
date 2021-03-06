#ifndef __MYPUT__
#define __MYPUT__

#include "../util/globals.h"

// Use these globals to manage what server should send back
extern int server_response_size, n;
extern char server_response[CHUNK_SIZE];

// Use these globals to manage what server should receive
extern int client_payload_size;
extern char client_payload[CHUNK_SIZE];

int my_put(int myargc, char *myargv[]);

#endif
