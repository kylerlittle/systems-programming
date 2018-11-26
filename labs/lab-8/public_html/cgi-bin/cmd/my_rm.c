#include "my_rm.h"

int my_rm(char *f1, char *f2) {
  unlink(f1);
}
