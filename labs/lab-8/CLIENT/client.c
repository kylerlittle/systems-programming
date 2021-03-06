// The client client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netdb.h>

#include "util/tokenize.h"
#include "lcmd.h"

#define MAX 256
#define CHUNK_SIZE 4096

// Define variables
struct hostent *hp;              
struct sockaddr_in  server_addr; 

int server_sock, r;
int SERVER_IP, SERVER_PORT; 

extern LCMD lcmd_table[];
char *l_cmd_argv[64];

// Use these globals to manage what client should receive
int server_response_size;
char server_response[CHUNK_SIZE];

// Use these globals to manage what client should send
int client_payload_size, n;
char client_payload[CHUNK_SIZE];

// client initialization code

int client_init(char *argv[])
{
  printf("======= client init ==========\n");
  
  printf("1 : get server info\n");
  hp = gethostbyname(argv[1]);
  if (hp==0){
    printf("unknown host %s\n", argv[1]);
    exit(1);
  }
  
  SERVER_IP   = *(long *)hp->h_addr;
  SERVER_PORT = atoi(argv[2]);
  
  printf("2 : create a TCP socket\n");
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock<0){
    printf("socket call failed\n");
    exit(2);
  }
  
  printf("3 : fill server_addr with server's IP and PORT#\n");
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = SERVER_IP;
  server_addr.sin_port = htons(SERVER_PORT);
  
  // Connect to server
  printf("4 : connecting to server ....\n");
  r = connect(server_sock,(struct sockaddr *)&server_addr, sizeof(server_addr));
  if (r < 0){
    printf("connect failed\n");
    exit(1);
  }
  
  printf("5 : connected OK to \007\n"); 
  printf("---------------------------------------------------------\n");
  printf("hostname=%s  IP=%s  PORT=%d\n", hp->h_name, inet_ntoa(SERVER_IP), SERVER_PORT);
  printf("---------------------------------------------------------\n");
  
  printf("========= init done ==========\n");
}

main(int argc, char *argv[ ])
{
  int n;
  char line[MAX], ans[MAX];
  
  if (argc < 3){
     printf("Usage : client ServerName ServerPort\n");
     exit(1);
  }
  
  client_init(argv);
  // sock <---> server
  printf("********  processing loop  *********\n");
  while (1) {
    printf("input a line : ");
    
    memset(line, 0, MAX);                // zero out line[ ]
    fgets(line, MAX, stdin);         // get a line (end with \n) from stdin

    line[strlen(line)-1] = 0;        // kill \n at end
    if (line[0]==0) exit(0);         // exit if NULL line
   
    client_payload_size = 0; 
       
    /* Tokenize. */
    char templine[MAX];
    strcpy(templine, line);  // make copy so as not to trash line
    n = tokenize(l_cmd_argv, templine, " ");
    
    /* Execute cmd on this side. Obviously, if cmd starts with 'l',
       simply 'continue' on loop after executing */
    int index = get_cmd_index(l_cmd_argv[0]);
      
    /* Execute command if valid. Otherwise, don't do anything */
    if (index != -1) {
	    lcmd_table[index].command_as_function(n-1, &l_cmd_argv[1]);
       clear_tok_list(l_cmd_argv);
       continue;  // executed local command, no need to interact with server
    }

    /* Ugly check for 'put' because tired. */   
    if (strcmp(l_cmd_argv[0], "put") == 0) {
       FILE *fp;
       int k;
       char buf[CHUNK_SIZE];

       fp=fopen(l_cmd_argv[1],"r");
       if (fp) {
          //CHUNK_SIZE is the blksize
          while(k=fread(buf,1,CHUNK_SIZE,fp)) {
             memcpy(client_payload, buf, k);
             client_payload_size+=k;
          }
       }
       fclose(fp);
    }
    
    /********* CLIENT WRITING *************
     **************************************
     * Client's messages will be of format:
     *   sizeOfMessage (in bytes)
     *   cmd arg1 arg2 ... argn
     *   contents (if any)
     **************************************/

    // First, send client_payload_size to client so they know what to expect
    char temp[MAX];
    sprintf(temp, "%d", client_payload_size);
    n = write(server_sock, temp, MAX);
    printf("client: notified server to expect message of %d bytes\n", client_payload_size);
    
    // Second, send cmd line over; keep track of bytes written to socket
    n = write(server_sock, line, MAX);
    printf("client: wrote n=%d bytes; ECHO=[%s]\n", strlen(line), line);
    
    // Lastly, write any extra contents if any.
    int total = 0, curr;
    while (total < client_payload_size) {
       memset(line, 0, MAX);
       memcpy(line, &client_payload[total], MAX);
       curr = write(server_sock, line, MAX);
       if (line[MAX-1] == '\0') total += strlen(line);
       else total += curr;
       printf("client: wrote n=%d bytes; ECHO=\n%*.*s\n", total, 0, MAX, line);
    }

    /******** READING FROM SERVER *********
     **************************************
     * Servers's messages will be of format:
     *   sizeOfMessage (in bytes)
     *   contents (if any)
     **************************************/

    // First, read first line from sock (message size)
    n = read(server_sock, ans, MAX);
    server_response_size = atoi(ans);
    if (server_response_size <= 0) {
      printf("server returned error code\n");
      continue;  // error on server-size, so continue...
    }
    printf("expecting message with %d bytes\n", server_response_size);
    
    /* Next, read the rest of the message in packets of size MAX. */
    total = 0;
    while (total < server_response_size) {
      memset(ans, 0, MAX);
      curr = read(server_sock, ans, MAX);
      memcpy(&server_response[total], ans, MAX);
      printf("%*.*s", 0, MAX, ans);
      if (ans[MAX-1] == '\0') total += strlen(ans);
      else total += curr;
    }

    /* Ugly check for 'get' because tired. */   
    if (strcmp(l_cmd_argv[0], "get") == 0) {
       FILE *fp;

       fp=fopen(l_cmd_argv[1],"w");
       if (fp) {
          fwrite(server_response, 1, server_response_size, fp);
          printf("Total bytes got from %s is: %d\n", l_cmd_argv[0], server_response_size);
       }
       fclose(fp);
    }

    /* Clear command token list */
    clear_tok_list(l_cmd_argv);

    // Kill server_response for next iteration
    memset(client_payload, 0, CHUNK_SIZE);
    client_payload_size = 0;

    printf("client: read total=%d bytes\n", total);
  }
}


