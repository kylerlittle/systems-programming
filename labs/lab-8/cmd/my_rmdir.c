#include "my_rmdir.h"

int my_rmdir(int myargc, char *myargv[]) {
  rmdir(myargv[0]);
}
