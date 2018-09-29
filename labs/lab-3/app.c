#include "app.h"

void run_sh_simulator(char* envp[]) {
    char _input_line[MAX_INPUT_LEN], *_args[MAX_ARGS] = {NULL};
    int i;
    /* Give first element a dummy value. */
    _args[0] = (char *)malloc(strlen("init") + 1);
    strcpy(_args[0], "init");

    /* Loop while first token isn't "exit" */
    while (strcmp(_args[0], "exit")) {
        get_input(_input_line);
        if (strlen(_input_line) != 0) {
            int n = tokenize(_args, _input_line, " ");
            execute_cmd(n, _args, envp);
        }
    }
    /* Before exiting, make sure to clear token list. */
    clear_tok_list(_args);
    exit(1);
}

int get_input(char *input_line) {
    printf("[klittle sh] : ");
    fgets(input_line, MAX_INPUT_LEN, stdin);
    input_line[strlen(input_line) - 1] = '\0';
}
