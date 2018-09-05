#include "app.h"

void run_file_system_simulator(void) {
  int index;
  char cmd[MAX_CMD_LEN], path[MAX_PATH_LEN], exit_cmd[MAX_CMD_LEN];

  /* The last command is the exit command. */
  strcpy(exit_cmd, cmds[NUM_CMDS - 1]);

  /* Loop while the command entered isn't the exit command. */
  while (strcmp(exit_cmd, cmd)) {
    /* Snag user input. */
    get_input(cmd, path);

    /* Find the index of the command in the table. */
    index = find_cmd(cmd);

    /* Execute command if valid. Otherwise, print error message. */
    if (is_valid_cmd(index))
      cmd_ptrs[index](cmd);
    else
      printf("%s: command not found\n", cmd);
  }
}

void get_input(char *cmd, char *path) {
  char line[MAX_INPUT_LEN];
  printf(">> ");
  fgets(line, MAX_INPUT_LEN, stdin);
  line[strlen(line) - 1] = '\0';
  sscanf(line, "%s %s", cmd, path);
}

int is_valid_cmd(int index) {
  return (index >= 0 && index < NUM_CMDS);
}