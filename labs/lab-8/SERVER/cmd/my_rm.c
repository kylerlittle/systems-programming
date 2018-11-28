#include "my_rm.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int my_rm(int myargc, char *myargv[]) {
  int status = unlink(myargv[0]);
  if (status < 0) sprintf(server_response, "unable to remove %s\n", myargv[0]);
  else sprintf(server_response, "removed %s successfully\n", myargv[0]);
  server_response_size = strlen(server_response);
}
