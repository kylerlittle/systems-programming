#include "app.h"

int run_cp_simulator(int argc, char* argv[]) {
    if (argc < 3) {
        /* Print usage and exit. */
        printf("Please run as \"$(executable_name) FILE/DIR1 FILE/DIR2\"\n");
        exit(1);
    }
    return myrcp(argv[1], argv[2]);
}


int myrcp(char *f1, char *f2)
{
    struct stat st1, st2;
    int result;

    /* 1a. If f1 doesn't exist, reject. */
    if ((result = stat(f1, &st1)) == ERROR_CODE) {
        printf("%s doesn't exist\n", f1);
        exit(1);
    }
    result = stat(f2, &st2);
    // /* 1b. f1 exists; if f1 is neither regular nor link, reject. */
    // if (!S_ISREG(st1.st_mode) || S_ISLNK(st1.st_mode)) {
    //     printf("%s is neither regular nor link\n", f1);
    //     exit(1);
    // }
    // /* 1c. If f2 exists and isn't a regular or is a link file, reject. */
    // if ((result = stat(f2, &st2)) != ERROR_CODE && (!S_ISREG(st1.st_mode) || S_ISLNK(st1.st_mode))) {
    //     printf("%s exists but isn't regular/link file\n", f2);
    //     exit(1);
    // }
    
    /* 2. If f1 is a regular file, continue processing. */
    if (S_ISREG(st1.st_mode)) {
        /* If f2 (recall result refers to file 2) doesn't exist or exists and is regular file. */
        if (result == ERROR_CODE || (result != ERROR_CODE && S_ISREG(st2.st_mode))) {
            // when we call cpf2f here, f2 may not exist...
            return cpf2f(f1, f2);
        } else { /* f2 exists and is a directory */
            return cpf2d(f1, f2);
        }
    }

    /* 3. If f1 is a directory file, continue processing. */
    if (S_ISDIR(st1.st_mode)) {
        /* If f2 exists and isn't a directory, we have a problem. Can't copy dir into file. */
        if (result != ERROR_CODE && !S_ISDIR(st2.st_mode)) {
            printf("Cannot copy dir %s into file %s\n", f1, f2);
            exit(1);
        }
        if (result == ERROR_CODE) { /* Doesn't exist, so make it, then recurse. */
            /* Use same permissions as f1. */
            if ((result = mkdir(f2, st1.st_mode)) == ERROR_CODE) {
                printf("%s doesn't exist; tried to create directory and failed\n", f2);
                exit(1);
            }
    	    return cpd2d(f1, f2);        
        }
    }
}

int cpf2f(char *f1, char *f2)
{
    //   1. reject if f1 and f2 are the SAME file
    struct stat st1, st2;
    int r1, r2, fd1, fd2 = INT_MAX, n, total = 0;
    char buf[BLOCK_SIZE];

    r1 = stat(f1, &st1);
    r2 = stat(f2, &st2);
    if (st1.st_ino == st2.st_ino && st1.st_dev == st2.st_dev) {
        printf("%s and %s are the same file\n", f1, f2);
        exit(1);
    }
    //   2. if f1 is LNK and f2 exists: reject
    if (S_ISLNK(st1.st_mode) && r2 != ERROR_CODE) {
        printf("%s is link and %s exists\n", f1, f2);
        exit(1);
    }
    //   3. if f1 is LNK and f2 does not exist: create LNK file f2 SAME as f1
    if (S_ISLNK(st1.st_mode) && r2 == ERROR_CODE) {
        fd2 = open(f2, O_WRONLY|O_CREAT|O_TRUNC, st1.st_mode);
        if (fd2 < 0) {
            printf("error creating %s\n", f2);
            exit(1);        // exit on error
        }
    }
    fd1 = open(f1, O_RDONLY);
    if (fd2 == INT_MAX) fd2 = open(f2, O_WRONLY|O_CREAT|O_TRUNC, st1.st_mode);
    //   4:
    //      open f1 for READ;
    //      open f2 for O_WRONLY|O_CREAT|O_TRUNC, mode=mode_of_f1;
    //      copy f1 to f2
    while ((n = read(fd1, buf, BLOCK_SIZE))) {
        write(fd2, buf, n);
        total += n;
    }
    if (DEBUG_MODE) printf("%d bytes copied from %s to %s\n", total, f1, f2);

    close(fd1);
    close(fd2);
}

int cpf2d(char *f1, char *f2)
{
    char _basename[MAX_FILENAME_LEN], result[MAX_FILENAME_LEN], _slash[MAX_FILENAME_LEN] = "/";

    /* Get full file path -- f2/_basename. Store in result. */
    strcpy(_basename, basename(f1));
    strcpy(result, f2);
    strcat(_slash,_basename);
    strcat(result, _slash);
    if (DEBUG_MODE) printf("Copying %s to %s\n", f1, result);

    /* Now, get file info of result, if it exists. */
    struct stat my_st;
    int r;

    /* Rather than doing a search (which complicates things unneccesarily by
     * having to add a check after looping to see whether file was found), utilize
     * the fact that 'stat' returns -1 when file isn't found.
     */
    if ((r = stat(result, &my_st)) == ERROR_CODE) {
        /* File wasn't found, so call cpf2f to create it. */
        cpf2f(f1, result);
    } else {
        /* If result is a file, copy f1 to f2 (i.e. override contents). */
        if (S_ISREG(my_st.st_mode)) cpf2f(f1, result);
        /* If result is a directory, then recurse so f1 is copied to that dir. */
        if (S_ISDIR(my_st.st_mode)) cpf2d(f1, result);
    }
}

int cpd2d(char *f1, char *f2)
{
    // recursively cp dir into dir    
}