#include "sh.h"

char *simple_cmd[] = { "my_cd", "my_exit", NULL };
void (*simple_cmd_ptrs[])(char *) = {(void (*)())my_cd, my_exit};
char HOME[MAX_ENV_VAR_LEN];
char PATH[MAX_ENV_VAR_LEN];

void execute_cmd(int myargc, char *myargv[], char* myenvp[]) {
    /* Check if cmd is simple and declare tokenized path array. */
    int index_of_simple_cmd = find_simple_cmd(myargv[0]), child_status;
    char *_paths[MAX_PATHS] = {NULL};

    /* Get env variables HOME and PATH. */
    get_env_var_val(find_env_var("HOME", myenvp), myenvp, HOME);
    get_env_var_val(find_env_var("PATH", myenvp), myenvp, PATH);

    /* Tokenize PATH. */
    tokenize(_paths, PATH, ":");

    /* If cmd is a simple command, don't fork. Execute in current proc. */
    if (index_of_simple_cmd != ERROR_CODE) {
        simple_cmd_ptrs[index_of_simple_cmd](myargv[1]);
    } else {
        /* Otherwise, fork a child proc, wait for the child to die, and print its exit code status. */
        int pid = fork();
        switch (return_fork_code(pid)) {
            case ERROR_CODE:
                printf("Forking a child process failed.\n");
                break;
            case CHILD_PROC:
                printf("I'm a child.\n");
                exit(100);
                break;
            case PARENT_PROC:
                printf("I'm mom.\n");
                pid = wait(&child_status);
                printf("Dead Child Process id=%d exit_code=%04x\n", pid, child_status);
                break;
        }
    }
    /* Before exiting, make sure to clear token list. */
    clear_tok_list(_paths);
}

int find_simple_cmd(char *cmd) {
    int i = 0;
    char my_cmd[3 + MAX_CMD_LEN];  // adding 3 characters ("my_") to maximum command length

    /* Prepend 'my_' string to get around C's inability to overload functions. */
    strcpy(my_cmd, "my_");
    strcat(my_cmd, cmd);

    while (simple_cmd[i]) {
        if (!strcmp(my_cmd, simple_cmd[i])) return i;
        i++;
    }
    return ERROR_CODE;
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

void my_cd(char *path) {
    chdir(path == NULL ? HOME : path);
}

void my_exit(char *path) {}

int find_env_var(char *env_var, char *envp[]) {
    int i = 0, env_var_len = strlen(env_var);
    while (envp[i]) {
        if (!strncmp(env_var, envp[i], env_var_len)) return i;
        i++;
    }
    return ERROR_CODE;
}

void get_env_var_val(int i, char *envp[], char *dest) {
    /* First, copy envp[i] into a new string so as not to destroy it. */
    char s[MAX_ENV_VAR_LEN];
    strcpy(s, envp[i]);

    /* Now, get string after "=" character and copy to dest. */
    char *env_var = strtok(s, "=");
    env_var = strtok(NULL, "=");
    strcpy(dest, env_var);
}