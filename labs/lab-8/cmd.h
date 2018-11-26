/*************** cmd.h file *********************************/
#ifndef __CMD__
#define __CMD__

/**** Included headers/libraries ****/
#include "cmd/my_ls.h"
#include "cmd/my_mkdir.h"
#include "cmd/my_rmdir.h"
#include "cmd/my_rm.h"
#include "cmd/my_cat.h"
#include "cmd/my_cp.h"
#include <stdio.h>
#include <string.h>

/**** Function Prototypes ****/
int get_cmd_index(char *cmd);

typedef struct command {
  char *command_as_string;  // char * since const
  int (*command_as_function)(int, char **);  // functions will just accept 2 strings for now
} CMD;

/* Table with cmd string and function address (CMD struct) as entries. */
CMD cmd_table[];

#endif
/*********************************************************/
