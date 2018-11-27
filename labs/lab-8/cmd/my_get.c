#include "my_get.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[4096];

int my_get(int myargc, char *myargv[]) {
  FILE *fp;
  int k;
  char buf[4096];

  fp=fopen(myargv[0],"r");
  if (fp) {
      //4096 is the blksize
      while(k=fread(buf,1,4096,fp)) {
        memcpy(server_response, buf, k);
	      server_response_size+=k;
      }
  }
  fclose(fp);
}
