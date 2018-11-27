#include "my_cat.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int my_cat(int myargc, char *myargv[]) {
  n = 0;
  if (myargv[0]!=NULL) {
    FILE *fp=fopen(myargv[0],"r");
    char ch;
    if(fp) {
      while((ch=fgetc(fp))!=EOF) {
	      sprintf(&server_response[n], "%c", ch);
        ++n;
      }
    }
    fclose(fp);
    server_response_size = n;
  }
}
