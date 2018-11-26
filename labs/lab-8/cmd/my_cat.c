#include "my_cat.h"

int my_cat(int myargc, char *myargv[]) {
  if (myargv[0]!=NULL) {
    FILE *fp=fopen(myargv[0],"r");
    char ch;
    if(fp) {
      while((ch=fgetc(fp))!=EOF) {
	      if (ch == '\n') printf("</br>");
	      else putchar(ch);
      }
    }
    fclose(fp);
  }
}
