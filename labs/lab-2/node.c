#include "node.h"

NODE *create_node(char *name, char type) {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    strcpy(newNode->name, name);
    newNode->type = type;
    newNode->child = newNode->sibling = newNode->parent = NULL;
    return newNode;
}

int delete_node(NODE *node) {
    if (DEBUG_MODE)
        printf("deleting node with name: %s\n", node->name);
    free(node);
    if (DEBUG_MODE)
        printf("successfully deleted node\n");
}

int kill_tree(NODE *tree) {
    if (tree) {
        kill_tree(tree->child);
        kill_tree(tree->sibling);
        delete_node(tree);
    }
}

int insert_end(NODE **front, NODE *node) {
    /* First, check if the list is empty. If so, insert here. */
    if (!(*front)) {
        *front = node;
    } else {
        NODE *p = *front;
        while (p->sibling)
            p = p->sibling;
        p->sibling = node;
    }
}

int remove_from_list(NODE *node) {
    NODE *parent = node->parent, *prev = NULL, *curr = NULL;
    curr = parent->child;

    /* Note: before this function is called, it's guaranteed that node is in the list. */
    while (curr != node) {
        prev = curr;
        curr = curr->sibling;
    }
    /* If prev is null, loop was never entered (i.e. node at front of list). */
    if (prev) {
        prev->sibling = curr->sibling;
    } else {
        parent->child = curr->sibling;
    }
    delete_node(curr);
}
