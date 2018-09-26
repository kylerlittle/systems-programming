#ifndef SH_EXECUTOR
#define SH_EXECUTOR

/* Standard Libraries and Header Files */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "constants.h"
#include "tokenize.h"

extern char *simple_cmd[];
extern void (*simple_cmd_ptrs[])(char*);
extern char HOME[];
extern char PATH[];

/* Function Signatures */
void execute_cmd(int myargc, char *myargv[], char* myenvp[]);
int find_simple_cmd(char *cmd);
int return_fork_code(int pid);
void my_cd(char *path);
void my_exit(char *path);
int find_env_var(char *env_var, char *envp[]);
void get_env_var_val(int i, char *envp[], char *dest);

#endif