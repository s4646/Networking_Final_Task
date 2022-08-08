#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define PORT 1337
#define SIM_LENGTH 10
/*
  When running the server, nothing happens at first.
  after changing the PORT to 1337 the server sends 10 packets before closing
  and prints "Server has written X to socket." for each packet.
*/

int main(void)
{ 
  // declare a new socket
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  socklen_t len;
  int count;
  
  // initialize the socket
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  // set all bytes of serv_name struct to zeros ("reset the struct")
  bzero(&serv_name, sizeof(serv_name)); 
  // set IP address type to IPv4
  serv_name.sin_family = AF_INET;
  // set port number                                   
  serv_name.sin_port = htons(PORT); 

  // assign the address specified by serv_name to sock
  bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name));
  // Prepare to accept connections on the socket.
  listen(sock, 1); 

  len = sizeof(serv_name);
  
  // Await a connection on the socket
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len);
  
  // for SIM_LENGTH times, print the sent packet ID
  for (count = 1; count <= SIM_LENGTH; count++)
    { write(connect_sock, &count, 4);
      printf("Server has written %d to socket.\n", count);
    }

  // close socket and connection
  close(connect_sock); 
  close(sock);

}
