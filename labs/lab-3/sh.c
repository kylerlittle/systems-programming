#include "sh.h"

char *simple_cmd[] = { "my_cd", "my_exit", NULL };
void (*simple_cmd_ptrs[])(char *) = {(void (*)())my_cd, my_exit};
char HOME[MAX_ENV_VAR_LEN];
char PATH[MAX_ENV_VAR_LEN];
int pipe_indices[MAX_PIPES] = { ERROR_CODE };
char *cmds_by_pipe[MAX_PIPES + 1][MAX_ARGS];

void execute_cmd(int myargc, char *myargv[], char* myenvp[]) {
    /* Check if cmd is simple and declare tokenized path array. */
    int num_pipes = 0, pid, pd[2], status;
    char *_paths[MAX_PATHS] = {NULL};

    /* Get env variables HOME and PATH. */
    get_env_var_val(find_env_var("HOME", myenvp), myenvp, HOME);
    get_env_var_val(find_env_var("PATH", myenvp), myenvp, PATH);

    /* Tokenize PATH. */
    tokenize(_paths, PATH, ":");

    /* First, check for pipes. */
    num_pipes = index_of_pipes(myargv, pipe_indices);
    split_cmds_by_pipes(pipe_indices, num_pipes, myargv, myargc, cmds_by_pipe);

    if (num_pipes > 0) {
        print_cmds_by_pipe(cmds_by_pipe, num_pipes);

        if (pipe(pd) == 0) fprintf(stderr, "creating pipe");
        pid = fork();
        switch(return_fork_code(pid)){
            case ERROR_CODE:
                fprintf(stderr, "exec() -- Forking a child process failed.\n");
                break;
            case CHILD_PROC:
                // Child -- Let this guy be the pipe writer.
                fprintf(stderr, "%d forked me; I am child %d -- execute_cmd()\n", getppid(), getpid());

                close(pd[0]);           // writer must close pipe reader
                close(STDOUT);          // close stdout 
                dup(pd[1]);             // replace stdout with pd[1] -- pipe writer

                /* Execute "head" */
                if (DEBUG_MODE) print_cmd(cmds_by_pipe[0]);
                exec(cmds_by_pipe[0], _paths, myenvp, true);
                break;
            case PARENT_PROC:
                // Parent -- Let this guy be the pipe reader.
                fprintf(stderr, "%d waiting for child -- execute_cmd() \n", getpid());
                pid = wait(&status);
                fprintf(stderr,"%d child proc died -- exit code = %d -- execute_cmd()\n", pid, status);

                close(pd[1]);           // reader must close pipe writer
                close(STDIN);           // close stdin
                dup(pd[0]);             // replace stdin with pd[0] -- pipe reader

                /* Execute "tail" -- could probably make a recursive call at this point
                if I wanted to handle multiple pipes... but I can barely handle one pipe
                at this point. */
                if (DEBUG_MODE) print_cmd(cmds_by_pipe[1]);
                exec(cmds_by_pipe[1], _paths, myenvp, false);
                break;
        }
    }
    else {
        if (DEBUG_MODE) print_cmd(cmds_by_pipe[0]);
        exec(cmds_by_pipe[0], _paths, myenvp, false);
    }
    /* Before exiting, make sure to clear token lists, reset fd[] to stdin & stdout. ORDER MATTERS! */
    close(pd[0]);
    dup(STDIN);
    close(pd[1]);
    dup(STDOUT);
    clear_cmds_by_pipe(cmds_by_pipe, num_pipes);
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
    strcpy(dest, strtok(NULL, "="));
}

int index_of_io_redirect(char *argv[]) {
    int i = 0;
    while (argv[i]) {
        if (!strncmp("<", argv[i], 1) || !strncmp(">", argv[i], 1)) return i;
        ++i;
    }
    return ERROR_CODE;
}

int get_io_redirect_code(char *io_redirect_op) {
    if (!strcmp(io_redirect_op, ">")) return WRITE;
    else if (!strcmp(io_redirect_op, "<")) return READ;
    else if (!strcmp(io_redirect_op, ">>")) return APPEND;
    else return ERROR_CODE;
}

void io_redirect(int code, char *filename) {
    switch (code) {
        case WRITE:
            close(STDOUT);
            open(filename, O_WRONLY|O_CREAT|O_TRUNC, 0644);
            break;
        case READ:
            close(STDIN);
            open(filename, O_RDONLY);
            break;
        case APPEND:
            close(STDOUT);
            open(filename, O_WRONLY|O_APPEND);
            break;
        case ERROR_CODE:
            fprintf(stderr, "Not a valid IO Redirect Operator\n");
            exit(100);  // kill myself -- might want to add macro for this constant
            break;
    }
}

int index_of_pipes(char *argv[], int pipe_indices[]) {
    int i = 0, num_pipes = 0;
    while (argv[i]) {
        if (!strcmp(argv[i], "|")) {
            pipe_indices[num_pipes++] = i;
        }
        ++i;
    }
    return num_pipes;
}

int split_cmds_by_pipes(int pipe_indices[], int num_pipes, char *argv[], int argc, char *cmds_by_pipe[][MAX_ARGS]) {
    int i, left = 0, j, k;

    for (i = 0; i < num_pipes; ++i) {
        /* Grab tokens between left and pipe_indices[i] */
        k = 0;
        for (j = left; j < pipe_indices[i]; ++j) {
            /* For each tok, copy allocate new memory and copy argv[j] over. */
            cmds_by_pipe[i][k] = (char *)malloc(strlen(argv[j] + 1));
            strcpy(cmds_by_pipe[i][k++], argv[j]);
        }
        /* End the list with NULL for convenience. */
        cmds_by_pipe[i][k] = NULL;
        
        /* Set new left to be first token after current pipe. */
        left = pipe_indices[i] + 1;
    }

    /* Grab the last group of commands after the last pipe. Yes this could be refactored. 
    TODO -- refactor this shit. Too fucking tired rn. */
    k = 0;
    for (j = left; j < argc; ++j) {
        /* For each tok, copy allocate new memory and copy argv[j] over. */
        cmds_by_pipe[i][k] = (char *)malloc(strlen(argv[j] + 1));
        strcpy(cmds_by_pipe[i][k++], argv[j]);
    }
    /* End the list with NULL for convenience. */
    cmds_by_pipe[i][k] = NULL;
}

int clear_cmds_by_pipe(char *cmds_by_pipe[][MAX_ARGS], int num_pipes) {
    int m, n;
    for (m = 0; m < num_pipes + 1; ++m) {
        clear_tok_list(cmds_by_pipe[m]);  // leverage code reuse
    }
}

int exec(char *argv[], char *_paths[], char *envp[], bool pipe_it_up) {
    int index_of_simple_cmd = find_simple_cmd(argv[0]), child_status, exec_status, path_len, i;

    /* If cmd is a simple command, don't fork. Execute in current proc. */
    if (index_of_simple_cmd != ERROR_CODE) {
        simple_cmd_ptrs[index_of_simple_cmd](argv[1]);
    } else {
        /* Otherwise, fork a child proc, wait for the child to die, and print its exit code status. */
        int pid = fork();
        switch (return_fork_code(pid)) {
            case ERROR_CODE:
                fprintf(stderr, "exec() -- Forking a child process failed.\n");
                break;
            case CHILD_PROC:
                fprintf(stderr, "%d forked me; I am child %d -- exec()\n", getppid(), getpid());
                char path_to_cmd[MAX_PATH_TO_CMD];
                i = 0;

                /*
                Some extra bullshit we're supposed to do first.
                */
                if (!strcmp(argv[0], "bash")) {
                    char buf[256];
                    FILE *fp;
                    if ((fp = fopen(argv[1], "r"))) {
                        fgets(buf, 256, fp);
                        if (!strncmp(buf, "#!", 2)) {
                            execve("/usr/bin/bash", argv, envp);
                        }
                    }
                }

                /* Loop while there are paths to check. */
                while (_paths[i]) {
                    /* Make a copy of the path so we can add "/$(cmd)" to it without modifying it directly. */
                    strcpy(path_to_cmd, _paths[i]);
                    path_len = strlen(path_to_cmd);
                    strcpy(&path_to_cmd[path_len], "/");
                    strcat(&path_to_cmd[path_len + 1], argv[0]);

                    /* Print path_to_cmd to check where we're searching for executable. */
                    if (DEBUG_MODE) fprintf(stderr, "Looking for %s at: %s\n", argv[0], path_to_cmd);

                    /* Attempt to execute the command. */
                    if (access(path_to_cmd, F_OK|X_OK) < 0) {
                        ++i;
                        continue;
                    } else {
                        /* First, check for IO redirect operator. */
                        int io_redirect_index = index_of_io_redirect(argv), io_redirect_code;
                        if (io_redirect_index != ERROR_CODE) {
                            /* Get io redirect code, and then handle the IO redirection. */
                            io_redirect_code = get_io_redirect_code(argv[io_redirect_index]);
                            io_redirect(io_redirect_code, argv[io_redirect_index + 1]);

                            /* Clear toks after i to not confuse execve. */
                            clear_toks_after_i(argv, io_redirect_index);
                        }
                        break;
                    }
                }
                fprintf(stderr, "exec() -- args to child proc: ");
                print_cmd(argv);
                fprintf(stderr, "____________________________________________________________________________________________\n");
                exec_status = execve(path_to_cmd, argv, envp);

                /* If we make it here, the command's executable simply wasn't found. */
                fprintf(stderr, "%s: command not found\n", argv[0]);
                exit(exec_status);
                break;
            case PARENT_PROC:
                fprintf(stderr, "%d waiting for child -- exec() \n", getpid());
                pid = wait(&child_status);
                fprintf(stderr, "____________________________________________________________________________________________\n");
                fprintf(stderr, "%d child proc died -- exit code = %d -- exec()\n", pid, child_status);
                
                /* Kill child writer if piping. */
                if (pipe_it_up) exit(0);
                break;
        }
    }    
}

void print_cmd(char *argv[]) {
    int i = 0;
    while (argv[i]) {
        fprintf(stderr, "%s ", argv[i++]);
    }
    fprintf(stderr, "\n");
}

void print_cmds_by_pipe(char *cmds[][MAX_ARGS], int num_pipes) {
    int m, n;
    for (m = 0; m < num_pipes + 1; ++m) {
        for (n = 0; cmds_by_pipe[m][n] != NULL; ++n) {
            fprintf(stderr, "%s ", cmds[m][n]);
        }
        fprintf(stderr, "\n");
    }
}