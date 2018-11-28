#include "my_lmkdir.h"

int my_lmkdir(int myargc, char *myargv[]) {
  int status = mkdir(myargv[0],0755);
  if (status < 0) printf("unable to create directory %s\n", myargv[0]);
  else printf("created %s successfully\n", myargv[0]);
}
