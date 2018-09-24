#include "app.h"

/*
A few problems...
1. empty string returns seg fault
*/

void run_sh_simulator(void) {
    char _input_line[MAX_INPUT_LEN], *_args[MAX_ARGS] = {NULL}, *tmp;

    /* Give first element a dummy value. */
    _args[0] = (char *)malloc(strlen("init") + 1);
    strcpy(_args[0], "init");

    while (strcmp(_args[0], "exit")) {
        printf("arg[0]: %s\n", _args[0]);
        printf("cmp: %d\n", strcmp(_args[0], "exit"));
        get_input(_input_line);
        if (strcmp(_input_line, "")) {
            printf("empty dawg");
        }
        printf("echo: %s\n", _input_line);
        int n = tokenize(_args, _input_line), i;
        printf("num args: %d\n", n);
        for (i = 0; i < n; ++i) {
            printf("%s\n", _args[i]);
        }
    }
    clear_tok_list(_args);
    exit(1);
}

int get_input(char *input_line) {
    printf(">>");
    fgets(input_line, MAX_INPUT_LEN, stdin);
    input_line[strlen(input_line) - 1] = '\0';
}

int tokenize(char *tok_list[], char *input_line) {
    char *s, *delimiters = " ";
    int n = 0;
    
    /* Clear tok_list */
    clear_tok_list(tok_list);

    s = strtok(input_line, delimiters);
    while (s) {
        tok_list[n] = (char*)malloc(strlen(s) + 1);
        strcpy(tok_list[n++], s);
        s = strtok(NULL, delimiters);
    }
    /* Mark the end of the tok list */
    tok_list[n] = NULL;
    return n;
}

int clear_tok_list(char *tok_list[]) {
    int i = 0;
    while (tok_list[i]) {
        free(tok_list[i]);
        ++i;
    }
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