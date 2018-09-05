#ifndef BASIC_FILE_SYSTEM_WRAPPER
#define BASIC_FILE_SYSTEM_WRAPPER

#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "constants.h"

void run_file_system_simulator(void);
void get_input(char *cmd, char *path);
int is_valid_cmd(int index);

#endif
