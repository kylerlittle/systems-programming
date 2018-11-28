#include "my_cp.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int my_cp(int myargc, char *myargv[]) {
  n = 0;
  FILE *fp, *gp;
  int k, amount = 0;
  char buf[CHUNK_SIZE];

  fp=fopen(myargv[0],"r");
  if(fp) {
    gp=fopen(myargv[1],"w");
    if(gp) {
      //CHUNK_SIZE is the blksize
      while(k=fread(buf,1,CHUNK_SIZE,fp)) {
	      fwrite(buf,1,k,gp);
	      amount+=k;
      }
      sprintf(server_response, "Total bytes copied to %s is: %d\n", myargv[1], amount);
      server_response_size = strlen(server_response);
    }
  }
  fclose(fp);
  fclose(gp);
}
