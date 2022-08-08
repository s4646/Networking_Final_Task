
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SIM_LENGTH 10 
#define IP_ADDRESS "127.0.0.1" 
#define PORT 1337 
/*
  When running the client, an error is printed:
  ** "Client is alive and establishing socket connection."
     "Error establishing communications: Connection refused" **
  After changing the IP_ADDRESS to 127.0.0.1 the program works, receives
  10 packets and prints: "Client has received X from socket." for each packet.

  When only the client is executed, a SYN request is sent through
  the socket but there is no server on the other side to reply with SYN-ACK
  so the connection is refused.
*/
int main(int argc, char* argv[])
{ 
  // declare a new socket
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 

  // declare new hostname and host address
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;

  printf("Client is alive and establishing socket connection.\n");
  
  // initialize the socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock);
      exit(1);
    }
      
  // set all bytes of cli_name struct to zeros ("reset the struct")
  bzero(&cli_name, sizeof(cli_name)); 
  // set IP address type to IPv4
  cli_name.sin_family = AF_INET;
  // insert IP address into the struct 
  hostname = argv[1];
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) {
  fprintf(stderr, "Error in resolving hostname %s\n", hostname);
  exit(1);
  }
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);
  cli_name.sin_addr.s_addr = inet_addr(hostaddr);
  // set port number
  cli_name.sin_port = htons(PORT);

  // connect the socket to the address specified by cli_name.
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error establishing communications");
      close(sock);
      exit(1);
    }

  // for SIM_LENGTH times, print the received packet ID
  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  // close socket and connection
  close(sock); 
  exit(0); 

} 
