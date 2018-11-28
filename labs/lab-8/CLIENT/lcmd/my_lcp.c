#include "my_lcp.h"

int my_lcp(int myargc, char *myargv[]) {
  int n = 0;
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
      printf("Total bytes copied to %s is: %d\n", myargv[1], amount);
    }
  }
  fclose(fp);
  fclose(gp);
}
