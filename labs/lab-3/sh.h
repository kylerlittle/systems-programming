#ifndef SH_EXECUTOR
#define SH_EXECUTOR

/* Standard Libraries and Header Files */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "constants.h"

extern char *simple_cmds[];

/* Function Signatures */
void execute_cmd(int myargc, char *myargv[], char* myenvp[]);
bool is_simple_cmd(char *cmd);
int return_fork_code(int pid);

#endif