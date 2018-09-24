#include "sh.h"

char *simple_cmds[] = { "cd", "exit", NULL };

void execute_cmd(int myargc, char *myargv[], char* myenvp[]) {
    /* If cmd is a simple command, don't fork. Execute in current proc. */
    if (is_simple_cmd(myargv[0])) {
        printf("I'm a simple command yo.\n");
    } else {
        /* Otherwise, fork a child proc, wait for the child to die, and print its exit code status. */
        printf("I'm complex.\n");
    }
    
}

bool is_simple_cmd(char *cmd) {
    int i = 0;
    while (simple_cmds[i]) {
        if (!strcmp(cmd, simple_cmds[i++])) return true;
    }
    return false;
}

int return_fork_code(int pid) {
    if (pid < 0) {
        return ERROR_CODE;
    } else if (pid == 0) {
        return CHILD_PROC;
    } else {
        return PARENT_PROC;
    }
}

// 1. write function to find str in envp and return its value (after '=' char)
        // call function to find HOME dir from myenvp & return its value
        // call function to get PATH
// 2. write function to tokenize PATH (delimiter is ':'). Can use same tokenize function as before.
//    add delimiter as a param and move function to new header/c file group.