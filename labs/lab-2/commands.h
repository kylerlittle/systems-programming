#ifndef FILE_SYSTEM_COMMANDS
#define FILE_SYSTEM_COMMANDS

#include <stdio.h>
#include <string.h>
#include "constants.h"

extern int (*cmd_ptrs[])(char*);
extern char *cmds[];

int find_cmd(char *cmd);
int mkdir(char*);
int rmdir(char*);
int ls(char*);
int cd(char*);
int pwd(char*);
int creat(char*);
int rm(char*);
int reload(char*);
int save(char*);
int menu(char*);
int quit(char*);

#endif