#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>


void printsin(struct sockaddr_in *s, char *str1, char *str2) {
  printf("%s\n", str1);
  printf("%s: ", str2);
  printf("ip = %s, ", inet_ntoa(s->sin_addr));
  printf("port = %d", s->sin_port);
  /*
  -- port: sin->sin_port (host integer type) 
  -- IP: sin->sin_addr (IP in dotted-decimal notation)
  */ 
  printf("\n");
}


int main(int argc, char *argv[])
{
  // declare a new socket
  int socket_fd, cc, fsize;
  // declare new structures describing an Internet socket address.
  struct sockaddr_in  s_in, from;
  // declare a new struct of a message
  struct { char head; u_long  body; char tail;} msg;
  
  // initilize the socket
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

  // set all bytes of s_in struct to zeros ("reset the struct")
  bzero((char *) &s_in, sizeof(s_in));  /* They say you must do this    */

  // set IP address type to IPv4
  s_in.sin_family = (short)AF_INET;
  // set IP address as host's IP address
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  // set port number
  s_in.sin_port = htons((u_short)0x3333);

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  // flush stdout
  fflush(stdout);
  // assign the address specified by s_in to socket_fd 
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));

  for(;;) {
    fsize = sizeof(from);
    // receive packet of N bytes into msg through the socket
    cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize);
    printsin( &from, "recv_udp: ", "Packet from:");
    // print packet
    printf("Got data ::%c%ld%c\n",msg.head,(long) ntohl(msg.body),msg.tail); 
    // flush stdout
    fflush(stdout);
  }
  
  return 0;
}

