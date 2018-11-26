#include "my_mkdir.h"

int my_mkdir(char *f1, char *f2) {
  mkdir(f1,0755);
}
