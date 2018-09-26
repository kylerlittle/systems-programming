#include "tokenize.h"

int tokenize(char *tok_list[], char *input_line, char *delimiters) {
    char *s;
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