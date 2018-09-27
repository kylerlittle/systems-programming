#ifndef CONSTANTS
#define CONSTANTS

/* Debugging Mode */
#define DEBUG_MODE 0

/* Macro -- String Lengths */
#define MAX_INPUT_LEN 128
#define MAX_ARGS 64
#define MAX_PATHS 64
#define MAX_CMD_LEN 32
#define MAX_ENV_VAR_LEN 256
#define MAX_PATH_TO_CMD 128
#define MAX_FILE_NAME_LEN 128
#define MAX_PIPES 64

/* Macro -- Status Codes */
#define ERROR_CODE -1
#define CHILD_PROC 0
#define PARENT_PROC 1

/* Macro -- IO Codes */
#define STDIN 0
#define STDOUT 1
#define STDERR 2

/* Macro -- IO Redirection Operators */
#define READ 0
#define WRITE 1
#define APPEND 2

#endif