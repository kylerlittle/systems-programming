#ifndef SH_SIMULATOR
#define SH_SIMULATOR

/* Standard Libraries and Header Files */   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "sh.h"

/* Function Signatures */
void run_sh_simulator(char* envp[]);
int get_input(char *input_line);
int tokenize(char *tok_list[], char *input_line);
int clear_tok_list(char *tok_list[]);

#endif