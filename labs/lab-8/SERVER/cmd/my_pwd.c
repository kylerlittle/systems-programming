#include "my_pwd.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int my_pwd(int myargc, char *myargv[]) {
  char *status = getcwd(server_response, CHUNK_SIZE);
  if (status == NULL) sprintf(server_response, "unable to print current working directory\n");
  else strcat(server_response, "\n");
  server_response_size = strlen(server_response);
}
