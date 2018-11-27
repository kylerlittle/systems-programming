#include "my_put.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[4096];

// Use these globals to manage what server should receive
int client_payload_size;
char client_payload[4096];

int my_put(int myargc, char *myargv[]) {
  FILE *fp;

  fp=fopen(basename(myargv[0]),"w");
  if (fp) {
    fwrite(client_payload,1,client_payload_size,fp);
    fclose(fp);
    sprintf(server_response, "Total bytes put in %s is: %d\n", myargv[0], client_payload_size);
    server_response_size = strlen(server_response);
  }
}
