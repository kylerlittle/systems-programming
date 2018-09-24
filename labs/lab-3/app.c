#include "app.h"

void run_sh_simulator(char* envp[]) {
    char _input_line[MAX_INPUT_LEN], *_args[MAX_ARGS] = {NULL}, *tmp;

    /* Give first element a dummy value. */
    _args[0] = (char *)malloc(strlen("init") + 1);
    strcpy(_args[0], "init");

    /* Loop while first token isn't "exit" */
    while (strcmp(_args[0], "exit")) {
        get_input(_input_line);
        if (strlen(_input_line) != 0) {
            int n = tokenize(_args, _input_line);
            execute_cmd(n, _args, envp);
        }
    }
    /* Before exiting, make sure to clear token list. */
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
        tok_list[i++] = NULL;
    }
}

