/************* myls.c file **********/
#include "my_ls.h"
//struct stat mystat, *sp;


int ls_file(char *fname)
{
  struct stat fstat, *sp;
  int r, i;
  char ftime[64];
  char linkname[256];
  char *t1 = "xwrxwrxwr-------";
  char *t2 = "----------------";
  sp = &fstat;
  if ( (r = lstat(fname, &fstat)) < 0){
    printf("can't stat %s</br>", fname); 
    return;
  }
  if ((sp->st_mode & 0xF000) == 0x8000)// if (S_ISREG())
    printf("%c",'-');
  if ((sp->st_mode & 0xF000) == 0x4000)// if (S_ISDIR())
    printf("%c",'d');
  if ((sp->st_mode & 0xF000) == 0xA000)// if (S_ISLNK())
    printf("%c",'l');
  for (i=8; i >= 0; i--){
    if (sp->st_mode & (1 << i))      // print r|w|x 
      printf("%c", t1[i]);
    else
      printf("%c", t2[i]);    // or print -
  }
  printf("%4d ",sp->st_nlink);  // link count
  printf("%4d ",sp->st_gid);  // gid
  printf("%4d ",sp->st_uid);  // uid
  printf("%8d ",sp->st_size);  // file size
  // print time
  strcpy(ftime, ctime(&sp->st_ctime));
  // print time in calendar form
  ftime[strlen(ftime)-1] = 0;  // kill \n at end
  printf("%s  ",ftime);
  // print name
  printf("%s", basename(fname));  //print file basename
  // print -> linkname if symbolic file
  if ((sp->st_mode & 0xF000)== 0xA000){
    // use readlink() to read linkname
    ssize_t r = readlink(fname, linkname, sp->st_size + 1);
    if (r >= 0 && r >= sp->st_size) printf(" -> %s", linkname);    // print linked name 
  }
  printf("</br>");
}

int my_ls(char *f1, char *f2) {
  struct dirent *ep;
  DIR *dp=opendir(".");   // cwd
  if(strlen(f1)){
      dp=opendir(f1);
  }
  while((ep=readdir(dp))!=NULL) {
    ls_file(ep->d_name);
  }
  printf("</br>");
}
