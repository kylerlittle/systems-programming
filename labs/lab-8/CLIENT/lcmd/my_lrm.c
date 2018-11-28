#include "my_lrm.h"

int my_lrm(int myargc, char *myargv[]) {
  int status = unlink(myargv[0]);
  if (status < 0) printf("unable to remove %s\n", myargv[0]);
  else printf("removed %s successfully\n", myargv[0]);
}
