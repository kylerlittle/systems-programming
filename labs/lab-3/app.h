#ifndef SH_SIMULATOR
#define SH_SIMULATOR

/* Standard Libraries and Header Files */   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "sh.h"
#include "tokenize.h"

/* Function Signatures */
void run_sh_simulator(char* envp[]);
int get_input(char *input_line);

#endif