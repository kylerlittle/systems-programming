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

/* Global Variables */
extern char *simple_cmd[];
extern void (*simple_cmd_ptrs[])(char*);
extern char HOME[];
extern char PATH[];
extern int pipe_indices[];
extern char *cmds_by_pipe[][MAX_ARGS];

/*********************** 
 * Function Signatures 
 ***********************/

/* Overall execution function (calls exec). Can have pipes. 
   @params
   myargc -- argc from this program's command line
   myargv -- argv from this program's command line
   myenvp -- typical envp */
void execute_cmd(int myargc, char *myargv[], char* myenvp[]);

/* Return index of cmd in simple_cmd function pointer array. ERROR_CODE if not simple.
   @params
   cmd -- any linux command */
int find_simple_cmd(char *cmd);

/* Return fork code from pid
   @params
   pid -- ERROR_CODE | CHILD_PROC | PARENT_PROC */
int return_fork_code(int pid);

/* Change current working directory if path supplied, else go to $(HOME).
   @params
   path -- absolute/relative directory path to change sh to. */
void my_cd(char *path);

/* Prints an exiting message. Exiting actually handled by app.c. 
   @params
   path -- we completely ignore this supplied argument. */
void my_exit(char *path);

/* Return index of env_var in envp or ERROR_CODE if not found.
   @params
   env_var -- string to hopefully find
   envp -- array of strings to search */
int find_env_var(char *env_var, char *envp[]);

/* Get string at envp[i]. Find value after "=" and place in dest.
   @params
   i -- index of envp to grab string
   envp -- array of strings to extract from
   dest -- dest char array to place result into */
void get_env_var_val(int i, char *envp[], char *dest);

/* Return the index of the io redirect operator in argv.
   @params:
   argv -- array of pointers to character arrays; array terminates in NULL pointer. */
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

/* Get indices of '|' strings in argv & place in pipe_indices.
   @params
   argv -- array of pointers to character arrays; array terminates in NULL pointer. 
   pipe_indices -- array of integers to place indices into. */
int index_of_pipes(char *argv[], int pipe_indices[]);

/* Place each string from argv into an array of pointers to string arrays.
   How? pipe_indices splits up argv and tells us which strings belong in which
   slice of cmds_by_pipe.
   @params:
   pipe_indices -- array indices of each '|' in argv
   num_pipes -- number of '|' tokens in argv
   argc -- length of argv
   cmds_by_pipe -- where to put copies of split up strings from argv
*/
int split_cmds_by_pipes(int pipe_indices[], int num_pipes, char *argv[], int argc, char *cmds_by_pipe[][MAX_ARGS]);

/* Clear cmds_by_pipe since its memory is on the heap.
   @params
   cmds_by_pipe -- copies of split up strings from argv
   num_pipes -- number of '|' tokens in argv */
int clear_cmds_by_pipe(char *cmds_by_pipe[][MAX_ARGS], int num_pipes);

/* Executes a command with no pipes but possible io redirection. 
   @params
   argv -- argv from command line of this program
   _paths -- tokenized $(PATH)
   envp -- typical envp
   pipe_it_up -- if true, kill process that called this function */
int exec(char *argv[], char *_paths[], char *envp[], bool pipe_it_up);

/* Debugging function to print a tokenized command 
   @params
   argv -- tokenized argv from command line of this program */
void print_cmd(char *argv[]);

/* Debugging function to print cmds_by_pipe
   @params
   cmds -- copies of split up strings from argv
   num_pipes -- number of '|' tokens in argv */
void print_cmds_by_pipe(char *cmds[][MAX_ARGS], int num_pipes);

#endif