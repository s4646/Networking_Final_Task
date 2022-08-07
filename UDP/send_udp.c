#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  // declare a new socket
  int socket_fd;
  // declare a new structure describing an Internet socket address.
  struct sockaddr_in  dest;
  // declare a new structure of description of data base entry for a single host.
  struct hostent *hostptr;
  // declare a new struct of a message buffer
  struct { char head; u_long body; char tail; } msgbuf;

  // initilize the socket
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
  // set all bytes of dest struct to zeros ("reset the struct")
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
  // get IP address from the terminal
  hostptr = gethostbyname(argv[1]);
  // set IP address type to IPv4
  dest.sin_family = (short) AF_INET;
  // insert IP address into the dest struct
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  // set port number
  dest.sin_port = htons((u_short)0x3333);

  msgbuf.head = '<';
  // convert the message from hostbyte order to network byte order.
  msgbuf.body = htonl(getpid()); /* IMPORTANT! */
  msgbuf.tail = '>';

  // Send packet of N bytes of msgbuf on socket to peer at dest
  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,
                  sizeof(dest));

  return 0;
}
