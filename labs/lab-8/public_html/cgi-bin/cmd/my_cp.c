#include "my_cp.h"

int my_cp(char *f1, char *f2) {
  FILE *fp, *gp;
  int n, amount = 0;
  char buf[4096];

  fp=fopen(f1,"r");
  if(fp) {
    gp=fopen(f2,"w");
    if(gp) {
      //4096 is the blksize
      while(n=fread(buf,1,4096,fp)) {
	fwrite(buf,1,n,gp);
	amount+=n;
      }
      printf("The total amount is: %d</br>", amount);
    }
  }
  fclose(fp);
  fclose(gp);
}
