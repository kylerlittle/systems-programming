#include "app.h"

int main(int argc, char* argv[], char* envp[]) {
  int i = 0;
  while (envp[i]) {
    printf("envp: %s\n", envp[i++]);
  }
  run_sh_simulator(envp);
  return 0;
}
