#ifndef SH_SIMULATOR
#define SH_SIMULATOR

/* Standard Libraries and Header Files */   
#include <stdio.h>       // for printf()
#include <stdlib.h>      // for exit()
#include <string.h>      // for strcpy(), strcmp(), etc.
#include <libgen.h>      // for basename(), dirname()
#include <fcntl.h>       // for open(), close(), read(), write()
#include <limits.h>      // for INT_MAX, INT_MIN, ...
#include <stdbool.h>

// for stat syscalls
#include <sys/stat.h>
#include <unistd.h>

// for opendir, readdir syscalls
#include <sys/types.h>
#include <dirent.h>
#include "constants.h"

/* Function Signatures */
int run_cp_simulator(int argc, char* argv[]);

/* cp wrapper -- calls cpf2d and cpd2d to recursively copy
 * file/dir f1 to file/dir f2.
 */
int myrcp(char *f1, char *f2);

/* Copy file f1 to file f2. */
int cpf2f(char *f1, char *f2);

/* Recursively copy file f1 to dir f2 */
int cpf2d(char *f1, char *f2);

/* Recursively copy dir f1 to dir f2. */
int cpd2d(char *f1, char *f2);

/* Copy "f2/basename(f1) into result. */
int f2slashbasef1(char *f1, char *f2, char *result);

#endif