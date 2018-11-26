#include "my_mkdir.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[4096];

int my_mkdir(int myargc, char *myargv[]) {
  int status = mkdir(myargv[0],0755);
  if (status < 0) sprintf(server_response, "unable to create directory %s\n", myargv[0]);
  else sprintf(server_response, "created %s successfully\n", myargv[0]);
  server_response_size = strlen(server_response);
}
