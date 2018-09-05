#include "commands.h"

/*
 * Define exported command list/table.
 * These arrays must retain the same ordering.
 */
int (*cmd_ptrs[])(char*) = {(int (*)())mkdir, rmdir, ls, cd, pwd, creat, rm,
                                 reload, save, menu, quit};
char *cmds[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
                "reload", "save", "menu", "quit", NULL};

int find_cmd(char *cmd) {
    int i = 0;

    /* Loop while cmds[i] isn't equal to NULL */
    while (cmds[i]) {
        if (!strcmp(cmd, cmds[i]))
            return i;
        i++;
    }
    return INVALID_INDEX;
}

int mkdir(char* path) {
    printf("path: %s\n", path);
}

int rmdir(char* path) {
    printf("path: %s\n", path);
}

int ls(char* path) {
    printf("path: %s\n", path);
}

int cd(char* path) {
    printf("path: %s\n", path);
}

int pwd(char* path) {
    printf("path: %s\n", path);
}

int creat(char* path) {
    printf("path: %s\n", path);
}

int rm(char* path) {
    printf("path: %s\n", path);
}

int reload(char* path) {
    printf("path: %s\n", path);
}

int save(char* path) {
    printf("path: %s\n", path);
}

int menu(char* path) {
    printf("path: %s\n", path);
}

int quit(char* path) {
    printf("path: %s\n", path);
}