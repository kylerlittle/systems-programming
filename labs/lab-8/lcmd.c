#include "lcmd.h"

LCMD lcmd_table[] = {
  {"lls",         &my_lls},
  {"lmkdir",      &my_lmkdir},
  {"lrmdir",      &my_lrmdir},
  {"lrm",         &my_lrm},
  {"lcat",        &my_lcat},
  {"lcp",         &my_lcp},
  {"lcd",         &my_lcd},
  {"lpwd",        &my_lpwd},
  {NULL,         NULL}
};

int get_cmd_index(char *cmd) {
  int i = 0;
  while (lcmd_table[i].command_as_string) {
    if (strcmp(cmd, lcmd_table[i].command_as_string) == 0) return i;
    i++;
  }
  return -1;
}
