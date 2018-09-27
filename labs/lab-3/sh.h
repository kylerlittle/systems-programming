#ifndef SH_EXECUTOR
#define SH_EXECUTOR

/* Standard Libraries and Header Files */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "constants.h"
#include "tokenize.h"

extern char *simple_cmd[];
extern void (*simple_cmd_ptrs[])(char*);
extern char HOME[];
extern char PATH[];
extern int pipe_indices[];
extern char *cmds_by_pipe[][MAX_ARGS];

/* Function Signatures */
void execute_cmd(int myargc, char *myargv[], char* myenvp[]);
int find_simple_cmd(char *cmd);
int return_fork_code(int pid);
void my_cd(char *path);
void my_exit(char *path);
int find_env_var(char *env_var, char *envp[]);
void get_env_var_val(int i, char *envp[], char *dest);
int index_of_io_redirect(char *argv[]);

/* Return the IO Redirect Macro based on string of redirection operator.
   @params:
   io_redirect_op -- string of io redirection operator (i.e. ">", "<", ">>") */
int get_io_redirect_code(char *io_redirect_op);

/* Perform IO Redirection with the given filename.
   @params:
   code -- READ | WRITE | APPEND 
   filename -- filename to redirect stdout/stdin/stderr to */
void io_redirect(int code, char *filename);

int index_of_pipes(char *argv[], int pipe_indices[]);

int split_cmds_by_pipes(int pipe_indices[], int num_pipes, char *argv[], char *cmds_by_pipe[][MAX_ARGS]);
int clear_cmds_by_pipe(char *cmds_by_pipe[][MAX_ARGS], int num_pipes);
int exec(char *argv[], char *_paths[], char *envp[]);

#endif