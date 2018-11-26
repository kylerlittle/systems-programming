#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "cmd.h"

#define MAX 10000

typedef struct {
    char *name;
    char *value;
} ENTRY;

extern CMD cmd_table[];
ENTRY entry[MAX];

main(int argc, char *argv[]) 
{
  int i, m, r, ch, n, status;
  int amount=0;
  DIR *dp;
  char buf[4096];
  FILE *fp;
  FILE *gp;
  struct dirent *ep;
  char cwd[128];

  m = getinputs();    // get user inputs name=value into entry[ ]
  getcwd(cwd, 128);   // get CWD pathname

  printf("Content-type: text/html\n\n");
  printf("<p>pid=%d uid=%d cwd=%s\n", getpid(), getuid(), cwd);

  printf("<H1>Echo Your Inputs</H1>");
  printf("You submitted the following name/value pairs:<p>");
 
  for(i=0; i <= m; i++)
     printf("%s = %s<p>", entry[i].name, entry[i].value);
  printf("<p>");


  /*****************************************************************
   Write YOUR C code here to processs the command
         mkdir dirname
         rmdir dirname
         rm    filename
         cat   filename
         cp    file1 file2
         ls    [dirname] <== ls CWD if no dirname
  *****************************************************************/

  // REFER TO PAGE 395 IN KC WANG SYSTEMS PROGRAMMING BOOK FOR HELPS AND HINTS (13.17.2)

  /* Find the index of the command in the table. */
  int index = get_cmd_index(entry[0].value);
        
  /* Execute command if valid. Otherwise, print error message. */
  if (index != -1)
    cmd_table[index].command_as_function(entry[1].value, entry[2].value);
  else
    printf("%s: command not found</br>", entry[0].value);

  // Add new line so stuff looks prettier!
  printf("</br>");

  // create a FORM webpage for user to submit again 
  printf("</title>");
  printf("</head>");
  printf("<body bgcolor=\"#FF0000\" link=\"#330033\" leftmargin=8 topmargin=8");
  printf("<p>------------------ DO IT AGAIN ----------------\n");
  
  printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~little/cgi-bin/mycgi.bin\">");

  //------ NOTE : CHANGE ACTION to YOUR login name ----------------------------
  //printf("<FORM METHOD=\"POST\" ACTION=\"http://cs360.eecs.wsu.edu/~YOURNAME/cgi-bin/mycgi.bin\">");
  
  printf("Enter command : <INPUT NAME=\"command\"> <P>");
  printf("Enter filename1: <INPUT NAME=\"filename1\"> <P>");
  printf("Enter filename2: <INPUT NAME=\"filename2\"> <P>");
  printf("Submit command: <INPUT TYPE=\"submit\" VALUE=\"Click to Submit\"><P>");
  printf("</form>");
  printf("------------------------------------------------<p>");

  printf("</body>");
  printf("</html>");
}
