#include "my_mkdir.h"

int my_mkdir(int myargc, char *myargv[]) {
  mkdir(myargv[0],0755);
}
