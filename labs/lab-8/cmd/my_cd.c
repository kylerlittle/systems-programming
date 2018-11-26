#include "my_cd.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[4096];

int my_cd(int myargc, char *myargv[]) {
  int status = chdir(myargv[0]);
  if (status < 0) sprintf(server_response, "unable to change to directory %s\n", myargv[0]);
  else sprintf(server_response, "changed to directory %s\n", getcwd());
  server_response_size = strlen(server_response);
}
