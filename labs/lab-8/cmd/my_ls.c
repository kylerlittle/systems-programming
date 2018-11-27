#include "my_ls.h"

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

int ls_file(char *fname)
{
  struct stat fstat, *sp;
  int r, i;
  char ftime[64];
  char linkname[256];
  char *t1 = "xwrxwrxwr-------";
  char *t2 = "----------------";
  sp = &fstat;
  if ( (r = stat(fname, &fstat)) < 0){
    sprintf(&server_response[n], "can't stat %s\n", fname);
    n = strlen(server_response);
    return;
  }
  if ((sp->st_mode & 0xF000) == 0x8000)// if (S_ISREG())
    sprintf(&server_response[n], "%c",'-');
  n = strlen(server_response);
  if ((sp->st_mode & 0xF000) == 0x4000)// if (S_ISDIR())
    sprintf(&server_response[n], "%c",'d');
  n = strlen(server_response);
  if ((sp->st_mode & 0xF000) == 0xA000)// if (S_ISLNK())
    sprintf(&server_response[n], "%c",'l');
  n = strlen(server_response);
  for (i=8; i >= 0; i--){
    if (sp->st_mode & (1 << i))      // print r|w|x 
      sprintf(&server_response[n], "%c", t1[i]);
    else
      sprintf(&server_response[n], "%c", t2[i]);    // or print -
    n = strlen(server_response);
  }
  sprintf(&server_response[n], "%4d ",sp->st_nlink);  // link count
  n = strlen(server_response);
  sprintf(&server_response[n], "%4d ",sp->st_gid);  // gid
  n = strlen(server_response);
  sprintf(&server_response[n], "%4d ",sp->st_uid);  // uid
  n = strlen(server_response);
  sprintf(&server_response[n], "%8d ",sp->st_size);  // file size
  n = strlen(server_response);
  // print time
  strcpy(ftime, ctime(&sp->st_ctime));
  // print time in calendar form
  ftime[strlen(ftime)-1] = 0;  // kill \n at end
  sprintf(&server_response[n], "%s  ",ftime);
  n = strlen(server_response);
  // print name
  sprintf(&server_response[n], "%s", basename(fname));  //print file basename
  n = strlen(server_response);
  // print -> linkname if symbolic file
  if ((sp->st_mode & 0xF000)== 0xA000){
    // use readlink() to read linkname
    ssize_t r = readlink(fname, linkname, sp->st_size + 1);
    if (r >= 0 && r >= sp->st_size) {
      sprintf(&server_response[n], " -> %s", linkname);    // print linked name 
      n = strlen(server_response);
    }
  }
  sprintf(&server_response[n], "\n");
  n = strlen(server_response);
}

int my_ls(int myargc, char *myargv[]) {
  n = 0;
  struct dirent *ep;
  DIR *dp=opendir(".");   // cwd
  if(myargv[0] != NULL){
      dp=opendir(myargv[0]);
  }
  while((ep=readdir(dp))!=NULL) {
    char full_path[256];
    strcpy(full_path, myargv[0] ? myargv[0] : ".");
    strcat(full_path, "/");
    strcat(full_path, ep->d_name);
    ls_file(full_path);
  }
  server_response_size = strlen(server_response);
}
