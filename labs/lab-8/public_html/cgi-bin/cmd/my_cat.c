#include "my_cat.h"

int my_cat(char *f1, char *f2) {
  if (f1!=NULL) {
    FILE *fp=fopen(f1,"r");
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
