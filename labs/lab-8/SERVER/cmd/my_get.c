#include "my_get.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int my_get(int myargc, char *myargv[]) {
  FILE *fp;
  int k;
  char buf[CHUNK_SIZE];

  fp=fopen(myargv[0],"r");
  if (fp) {
      //CHUNK_SIZE is the blksize
      while(k=fread(buf,1,CHUNK_SIZE,fp)) {
        memcpy(server_response, buf, k);
	      server_response_size+=k;
      }
  }
  fclose(fp);
}
