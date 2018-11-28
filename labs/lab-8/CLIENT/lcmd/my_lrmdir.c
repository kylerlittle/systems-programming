#include "my_lrmdir.h"

int my_lrmdir(int myargc, char *myargv[]) {
  int status = rmdir(myargv[0]);
  if (status < 0) printf("unable to remove directory %s\n", myargv[0]);
  else printf("removed %s successfully\n", myargv[0]);
}
