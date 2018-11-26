#include "cmd.h"

CMD cmd_table[] = {
  {"ls",         &my_ls},
  {"mkdir",      &my_mkdir},
  {"rmdir",      &my_rmdir},
  {"rm",         &my_rm},
  {"cat",        &my_cat},
  {"cp",         &my_cp},
  {"cd",         &my_cd},
  {"pwd",        &my_pwd},
  {NULL,         NULL}
};

int get_cmd_index(char *cmd) {
  int i = 0;
  while (cmd_table[i].command_as_string) {
    if (strcmp(cmd, cmd_table[i].command_as_string) == 0) return i;
    i++;
  }
  return -1;
}
