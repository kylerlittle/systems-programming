#include "sh.h"

char *simple_cmd[] = { "my_cd", "my_exit", NULL };
void (*simple_cmd_ptrs[])(char *) = {(void (*)())my_cd, my_exit};
char HOME[MAX_ENV_VAR_LEN];
char PATH[MAX_ENV_VAR_LEN];
int pipe_indices[MAX_PIPES] = { ERROR_CODE };
char *cmds_by_pipe[MAX_PIPES + 1][MAX_ARGS];

void execute_cmd(int myargc, char *myargv[], char* myenvp[]) {
    /* Check if cmd is simple and declare tokenized path array. */
    int index_of_simple_cmd = find_simple_cmd(myargv[0]), child_status, num_pipes = 0;
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
                printf("Inside Child Process pid=%d\n", getpid());
                /* First, check for pipes. */
                num_pipes = index_of_pipes(myargv, pipe_indices);
                split_cmds_by_pipes(pipe_indices, num_pipes, myargv, cmds_by_pipe);
                if (num_pipes > 0) {
                    int pd[2];

                    pipe(pd);        // creates a PIPE; pd[0] for READ  from the pipe, 
                                        //                 pd[1] for WRITE to   the pipe.

                    pid = fork();    // fork a child process
                                        // child also has the same pd[0] and pd[1]

                    if (pid) {        // parent as pipe pipe WRITER
                        close(pd[0]); // WRITER MUST close pd[0]

                        close(1);     // close 1
                        dup(pd[1]);   // replace 1 with pd[1]
                        close(pd[1]); // close pd[1] since it has replaced 1
                        exec(cmds_by_pipe[0], _paths, myenvp);   // change image to cmd1
                    }
                    else {            // child as pipe pipe READER
                        close(pd[1]); // READER MUST close pd[1]

                        close(0);  
                        dup(pd[0]);   // replace 0 with pd[0]
                        close(pd[0]); // close pd[0] since it has replaced 0
                        exec(cmds_by_pipe[1], _paths, myenvp);   // change image to cmd2
                    }
                }
                else {
                    exec(myargv, _paths, myenvp);
                }
                // int m, n;
                // for (m = 0; m < num_pipes + 1; ++m) {
                //     printf("pipe %d\n", m + 1);
                //     for (n = 0; cmds_by_pipe[m][n] != NULL; ++n) {
                //         printf("tok: %s\n", cmds_by_pipe[m][n]);
                //     }
                // }
                /*
                    Process Flow:
                    1. tokenize
                    2. try cmd in each PATH dir (_paths[0 ... len(_paths)])
                    3. check for pipes
                    4. execute each subcmd within pipes (don't need yet)
                    5. for each subcmd, check for IO redirection. Basically, if second to last tok
                        within subcmd is <, >, or >>
                */
                /* check for IO redirection -- we have '<' for replacing stdin and reading from infile,
                '>' for replacing stdout & writing to outfile, and '>>' for appending to an outfile

                0 is stdin, 1 is stdout, 2 is stderr
                To perform IO redirection, close(0) & open(filename, 0_RDONLY);
                Other codes: 0_RDONLY, 0_WRONLY, 0_APPEND
                */

                /* First, check for IO redirect operator. */
                // int io_redirect_index = index_of_io_redirect(myargv), io_redirect_code;
                // if (io_redirect_index != ERROR_CODE) {
                //     /* Get io redirect code, and then handle the IO redirection. */
                //     io_redirect_code = get_io_redirect_code(myargv[io_redirect_index]);
                //     io_redirect(io_redirect_code, myargv[io_redirect_index + 1]);
                //     clear_toks_after_i(myargv, io_redirect_index);
                // }
                
                // int i = 0, path_len;
                // char path_to_cmd[MAX_PATH_TO_CMD];
                
                // /* Loop while there are paths to check. */
                // while (_paths[i]) {
                //     /* Make a copy of the path so we can add "/$(cmd)" to it without modifying it directly. */
                //     strcpy(path_to_cmd, _paths[i]);
                //     path_len = strlen(path_to_cmd);
                //     strcpy(&path_to_cmd[path_len], "/");
                //     strcat(&path_to_cmd[path_len + 1], myargv[0]);

                //     /* Print path_to_cmd to check where we're searching for executable. */
                //     if (DEBUG_MODE) printf("Looking for %s at: %s\n", myargv[0], path_to_cmd);

                //     /* Attempt to execute the command. */
                //     execve(path_to_cmd, myargv, myenvp);
                //     ++i;
                // }
                /* If we make it here, the command's executable wasn't found. */
                printf("%s: command not found\n", myargv[0]);
                exit(100);
                break;
            case PARENT_PROC:
                printf("Inside Parent Process pid=%d\n", getpid());
                pid = wait(&child_status);
                printf("Dead Child Process pid=%d exit_code=%04x\n", pid, child_status);
                break;
        }
    }
    /* Before exiting, make sure to clear token list. */
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
    env_var = strtok(NULL, "=");
    strcpy(dest, env_var);
}

/* 
 1. Need a function that can check for '>', '<', and '>>' and return the index of it.
        Then index + 1 will be the location of the filename, and arr[i] will be the type
    int index_of_io_redirect(char *argv[]);
    // if !strncmp("<", io_redirect_op, 1) || !strncmp(">", io_redirect_op, 1)) return i        
    // return ERROR_CODE
 2. Need a function that can check for '|' and return an array of indices of the pipes.
    -- consequently, need a function that can split up myargv[] into sub_argv[]
 3. Need a function that performs the correct IO redirection based on IO Redirect Code & filename
    


    int get_io_redirect_code(char *io_redirect_op) // return APPEND | READ | WRITE based on cmp

    void io_redirect(int code, char *filename);
*/

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


/*
Need someway to make execve ignore ">" and filename. Probs have to deallocate the mem for those.
Add a function to do that? From index i and on?
*/
void io_redirect(int code, char *filename) {
    switch (code) {
        case WRITE:
            close(STDOUT);
            open(filename, O_WRONLY|O_CREAT, 0644);
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
            printf("Not a valid IO Redirect Operator\n");
            exit(100);  // kill myself -- might want to add macro for this constant
            break;
    }
}

/* Need a way to close the io_redirect stream (i.e. reset it).
   It appears it does this automatically, but I should close the original stream to be safe.
   Otherwise, we might have corrupt memory. 
   Actually... I'm thinking that the kernel probably handles this. It probably resets
   fd[] when a child dies and ensures file streams are closed properly... */

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

// function to essentially tokenize the already tokenized argv
// give pipe_indices, number of pipes, argv, and an array of char pointers
// cmds_by_pipe should be an array of char ** or *char[]
int split_cmds_by_pipes(int pipe_indices[], int num_pipes, char *argv[], char *cmds_by_pipe[][MAX_ARGS]) {
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

    /* Grab the last group of commands after the last pipe. Yes this could be refactored. */
    k = 0;
    for (j = left; argv[j] != NULL; ++j) {
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
        for (n = 0; cmds_by_pipe[m][n] != NULL; ++n) {
            free(cmds_by_pipe[m][n]);
        }
    }
}

int exec(char *argv[], char *_paths[], char *envp[]) {
    /* First, check for IO redirect operator. */
    int io_redirect_index = index_of_io_redirect(argv), io_redirect_code;
    if (io_redirect_index != ERROR_CODE) {
        /* Get io redirect code, and then handle the IO redirection. */
        io_redirect_code = get_io_redirect_code(argv[io_redirect_index]);
        io_redirect(io_redirect_code, argv[io_redirect_index + 1]);
        clear_toks_after_i(argv, io_redirect_index);
    }
    
    int i = 0, path_len;
    char path_to_cmd[MAX_PATH_TO_CMD];
    
    /* Loop while there are paths to check. */
    while (_paths[i]) {
        /* Make a copy of the path so we can add "/$(cmd)" to it without modifying it directly. */
        strcpy(path_to_cmd, _paths[i]);
        path_len = strlen(path_to_cmd);
        strcpy(&path_to_cmd[path_len], "/");
        strcat(&path_to_cmd[path_len + 1], argv[0]);

        /* Print path_to_cmd to check where we're searching for executable. */
        if (DEBUG_MODE) printf("Looking for %s at: %s\n", argv[0], path_to_cmd);

        /* Attempt to execute the command. */
        execve(path_to_cmd, argv, envp);
        ++i;
    }
}