#include "my_rm.h"

int my_rm(int myargc, char *myargv[]) {
  unlink(myargv[0]);
}
