#ifndef BINARY_NODE
#define BINARY_NODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

typedef struct node {
    char name[MAX_PATH_LEN];
    char type;
    struct node *child, *sibling, *parent;
} NODE;

NODE *create_node(char *name, char type);
int delete_node(NODE *node);
int kill_tree(NODE *top);
int insert_end(NODE **front, NODE *node);
int remove_from_list(   NODE *node);

#endif