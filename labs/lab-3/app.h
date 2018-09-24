#ifndef SH_SIMULATOR
#define SH_SIMULATOR

/* Standard Libraries and Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Macro -- String Lengths */
#define MAX_INPUT_LEN 128
#define MAX_ARGS 64

/* Macro -- Status Codes */
#define ERROR_CODE -1
#define CHILD_PROC 0
#define PARENT_PROC 1

/* Function Signatures */
void run_sh_simulator(void);
int get_input(char *input_line);
int tokenize(char *tok_list[], char *input_line);
int clear_tok_list(char *tok_list[]);
int return_fork_code(int pid);

#endif