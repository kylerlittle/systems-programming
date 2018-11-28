/*************** cmd.h file *********************************/
#ifndef __LCMD__
#define __LCMD__

/**** Included headers/libraries ****/
#include "lcmd/my_lls.h"
#include "lcmd/my_lmkdir.h"
#include "lcmd/my_lrmdir.h"
#include "lcmd/my_lrm.h"
#include "lcmd/my_lcat.h"
#include "lcmd/my_lcp.h"
#include "lcmd/my_lcd.h"
#include "lcmd/my_lpwd.h"
#include <stdio.h>
#include <string.h>

/**** Function Prototypes ****/
int get_cmd_index(char *cmd);

typedef struct command {
  char *command_as_string;  // char * since const
  int (*command_as_function)(int, char **);  // functions will just accept 2 strings for now
} LCMD;

/* Table with cmd string and function address (CMD struct) as entries. */
LCMD lcmd_table[];

#endif
/*********************************************************/