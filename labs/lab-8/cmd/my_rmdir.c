#include "my_rmdir.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[4096];

int my_rmdir(int myargc, char *myargv[]) {
  int status = rmdir(myargv[0]);
  if (status < 0) sprintf(server_response, "unable to remove directory %s\n", myargv[0]);
  else sprintf(server_response, "removed %s successfully\n", myargv[0]);
  server_response_size = strlen(server_response);
}
