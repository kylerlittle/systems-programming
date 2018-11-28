#include "my_lcd.h"

int my_lcd(int myargc, char *myargv[]) {
  int status = chdir(myargv[0]);
  if (status < 0) printf("unable to change to directory %s\n", myargv[0]);
  else printf("changed to directory %s\n", getcwd());
}
