#include "my_lpwd.h"

int my_lpwd(int myargc, char *myargv[]) {
  char buf[256];
  char *status = getcwd(buf, 256);
  if (status == NULL) printf("unable to print current working directory\n");
  else printf("%s\n", buf);
}
