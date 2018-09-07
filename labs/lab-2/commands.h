#ifndef FILE_SYSTEM_COMMANDS
#define FILE_SYSTEM_COMMANDS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"
#include "node.h"

extern int (*cmd_ptrs[])(char*);
extern char *cmds[];
NODE *root, *cwd, *start;
extern char *tok_list[];

/* File tree functions. */
int init(void);
int tokenize(char *pathname);
int clear_tok_list(void);
NODE *search_for_child(NODE *parent, char *name);
NODE *path_to_node(char *pathname);

/* File tree commands. */
int find_cmd(char *cmd);
int mkdir(char*);
int rmdir(char*);
int ls(char*);
int cd(char*);
int pwd(char*);
int rpwd(NODE* curr);
int creat(char*);
int rm(char*);
int reload(char*);
int save(char*);
int menu(char*);
int quit(char*);

#endif