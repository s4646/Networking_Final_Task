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
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // declare a new socket
    int socket_fd, fsize, cc;
    // declare new structures describing an Internet socket address.
    struct sockaddr_in s_in, s_out;
    // declare a new struct of a message buffer
    struct { char head; u_long body; char tail; } msg;
    // declare a new structure of description of data base entry for a single host.
    struct hostent *hostptr;

    // initilize the socket
    socket_fd = socket (AF_INET, SOCK_DGRAM, 0);
    // set all bytes of dest struct to zeros ("reset the struct")
    bzero((char *) &s_in, sizeof(s_in)); /* They say you must do this */
    bzero((char *) &s_out, sizeof(s_out)); /* They say you must do this */

    // get IP address from the terminal
    hostptr = gethostbyname(argv[1]);
    
    // set IP address type to IPv4
    s_in.sin_family = (short) AF_INET;
    s_out.sin_family = (short) AF_INET;

    // insert IP address into the dest struct
    bcopy(hostptr->h_addr, (char *)&s_out.sin_addr,hostptr->h_length);
    
    // set port number
    char* ptr;
    s_in.sin_port = (u_short)strtol(argv[2], &ptr, 10);
    s_out.sin_port = (u_short)strtol(argv[2], &ptr, 10) + 1;
    
    // assign the address specified by s_in to socket_fd 
    bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));
    srandom(42);
    for(;;)
    {
        fsize = sizeof(s_in);
        // receive packet of N bytes into msg through the socket
        cc = recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&s_in,&fsize);
        double temp = ((float)random())/((float)RAND_MAX);
        if (temp>0.5)
        {
            // Send packet of N bytes of msgbuf on socket to peer at dest
            sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&s_out, sizeof(s_out));
        }
        // reset msg
        bzero((char *) &msg, sizeof(msg));
    }

    return 0;
}