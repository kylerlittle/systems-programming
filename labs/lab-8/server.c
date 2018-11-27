// This is the echo SERVER server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netdb.h>

#include "util/tokenize.h"
#include "cmd.h"

#define MAX 256
#define CHUNK_SIZE 4096

// Define variables:
struct sockaddr_in  server_addr, client_addr, name_addr;
struct hostent *hp;

int  mysock, client_sock;              // socket descriptors
int  serverPort;                     // server port number
int  r, length;                   // help variables

extern CMD cmd_table[];
char *cmd_argv[64];

// Use these globals to manage what server should send back
int server_response_size, n;
char server_response[CHUNK_SIZE];

// Use these globals to manage what server should receive
int client_payload_size;
char client_payload[CHUNK_SIZE];

// Server initialization code:

int server_init(char *name)
{
  printf("==================== server init ======================\n");   
  // get DOT name and IP address of this host
  
  printf("1 : get and show server host info\n");
  hp = gethostbyname(name);
  if (hp == 0){
    printf("unknown host\n");
    exit(1);
  }
  printf("    hostname=%s  IP=%s\n", hp->h_name,  inet_ntoa(*(long *)hp->h_addr));
  
  //  create a TCP socket by socket() syscall
  printf("2 : create a socket\n");
  mysock = socket(AF_INET, SOCK_STREAM, 0);
  if (mysock < 0){
    printf("socket call failed\n");
    exit(2);
  }
  
  printf("3 : fill server_addr with host IP and PORT# info\n");
  // initialize the server_addr structure
  server_addr.sin_family = AF_INET;                  // for TCP/IP
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // THIS HOST IP address  
  server_addr.sin_port = 0;   // let kernel assign port
  
  printf("4 : bind socket to host info\n");
  // bind syscall: bind the socket to server_addr info
  r = bind(mysock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
    printf("bind failed\n");
    exit(3);
  }
  
  printf("5 : find out Kernel assigned PORT# and show it\n");
  // find out socket port number (assigned by kernel)
  length = sizeof(name_addr);
  r = getsockname(mysock, (struct sockaddr *)&name_addr, &length);
  if (r < 0){
    printf("get socketname error\n");
    exit(4);
  }
  
  // show port number
  serverPort = ntohs(name_addr.sin_port);   // convert to host ushort
  printf("    Port=%d\n", serverPort);
  
  // listen at port with a max. queue of 5 (waiting clients) 
  printf("5 : server is listening ....\n");
  listen(mysock, 5);
  printf("===================== init done =======================\n");
}


main(int argc, char *argv[])
{
  char *hostname;
  char line[MAX];
  char cwd[MAX];
  int curr, total;
  
  if (argc < 2)
    hostname = "localhost";
  else
    hostname = argv[1];
  
  char *blah = getcwd(cwd, MAX);       
  server_init(hostname);
  chroot(cwd); // NOT SECURE -- doesn't cover all bases: see http://man7.org/linux/man-pages/man2/chroot.2.html
  printf("server: chroot to %s\n", cwd);
  fflush(stdout);
  
  // Try to accept a client request
  while(1) {
    printf("server: accepting new connection ....\n"); 
    
    // Try to accept a client connection as descriptor newsock
    length = sizeof(client_addr);
    client_sock = accept(mysock, (struct sockaddr *)&client_addr, &length);
    if (client_sock < 0){
      printf("server: accept error\n");
      exit(1);
    }
    printf("server: accepted a client connection from\n");
    printf("-----------------------------------------------\n");
    printf("        IP=%s  port=%d\n", inet_ntoa(client_addr.sin_addr.s_addr),
	   ntohs(client_addr.sin_port));
    printf("-----------------------------------------------\n");
    
    // Processing loop: newsock <----> client
    while(1) {
       memset(line, 0, MAX);                // zero out line[ ]
      /******** READING FROM CLIENT *********
       **************************************
       * Client's messages will be of format:
       *   sizeOfMessage (in bytes)
       *   cmd arg1 arg2 ... argn
       *   contents (if any)
       **************************************/
      
      // First, read first line from sock (message size)
      n = read(client_sock, line, MAX);
      
      if (n == 0) {
        printf("server: client died, server loops\n");
        close(client_sock);
        break;
      }
      client_payload_size = atoi(line);
      
      // Second, receive cmd line
      memset(line, 0, MAX);
      n = read(client_sock, line, MAX);
      printf("server: read n=%d bytes; ECHO=[%s]\n", n, line);
      /* Tokenize. */
      n = tokenize(cmd_argv, line, " ");  // NOTE -- line is garbage after this
      /* Print off what was tokenized. */
      int i = 0;
      while (i < n) {
	      printf("argv[%d]=%s\n", i, cmd_argv[i]);
	      i++;
      }

      if (client_payload_size == 0) {
         printf("No additional content to collect\n");
      } else {
         printf("expecting message with %d bytes\n", client_payload_size);

         // Lastly, read the rest of the message in packets of size MAX.
         total = 0;
         while (total < client_payload_size) {
            memset(line, 0, MAX);
            curr = read(client_sock, line, MAX);
            memcpy(&client_payload[total], line, MAX);
            if (line[MAX-1] == '\0') total += strlen(line);
            else total += curr;
            printf("server: read n=%d bytes; ECHO=\n%*.*s\n", total, 0, MAX, line);
         }
      }
      
      /* Find the index of the command in the table. */
      int index = get_cmd_index(cmd_argv[0]);
      
      /* Execute command if valid. Otherwise, print error message. */
      if (index != -1)
	      cmd_table[index].command_as_function(n-1, &cmd_argv[1]);
      else
	      printf("%s: command not found\n", cmd_argv[0]);
      
      /* Clear command token list */
      clear_tok_list(cmd_argv);


      /*********** SERVER WRITING ***********
       **************************************
       * Servers's messages will be of format:
       *   sizeOfMessage (in bytes)
       *   contents (if any)
       **************************************/

      /* First, send server_response_size to client so they know what to expect */
      memset(line, 0, MAX);
      sprintf(line, "%d", server_response_size);
      n = write(client_sock, line, MAX);
      printf("server: notified client to expect message of %s bytes\n", line);
      
      /* Now, write the rest of the message in packets of size MAX. */
      total = 0;
      while (total < server_response_size) {
         memset(line, 0, MAX);
         memcpy(line, &server_response[total], MAX);
         curr = write(client_sock, line, MAX);
         if (line[MAX-1] == '\0') total += strlen(line);
         else total += curr;
         printf("server: wrote n=%d bytes; ECHO=\n%*.*s\n", total, 0, MAX, line);
      }
      
      // Kill server_response for next iteration
      memset(server_response, 0, CHUNK_SIZE);
      server_response_size = 0;
      
      printf("server: total bytes written: %d\n", total);
      printf("server: ready for next request\n");
    }
  }
}

