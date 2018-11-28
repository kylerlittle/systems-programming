#include "my_lcat.h"

int my_lcat(int myargc, char *myargv[]) {
  int n = 0;
  if (myargv[0]!=NULL) {
    FILE *fp=fopen(myargv[0],"r");
    char ch;
    if(fp) {
      while((ch=fgetc(fp))!=EOF) {
	      printf("%c", ch);
        ++n;
      }
    }
    fclose(fp);
  }
}
