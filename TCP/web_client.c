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

void parseCMD(char** host, char** path, char** port, char** protocol, int argc, char* argv[])
{
    if(argc-1 == 0)
    {
        printf("Invalid argument\n");
        exit(1);
    }
    int i = 1;
    *path = (char*)malloc(1);
    char* tok = strtok(argv[1], "/");

    while(tok != NULL)
    {
        // insert protocol into protocol pointer
        if(i==1)
        {
            *protocol = (char*)calloc(strlen(tok)+1, 1);
            strcat(*protocol, tok);
            (*protocol)[strlen(tok)-1] = '\0';
        }
        // insert hostname + port into host pointer
        else if(i==2)
        {
            *host = (char*)calloc(strlen(tok)+1, 1);
            strcat(*host, tok);
        }
        // insert path into path pointer
        else
        {
            if(strcmp(tok, "(null)") != 0)
            {
                *path = (char*)realloc(*path, strlen(*path) + strlen(tok) + 1);
                strcat(*path, "/");
                strcat(*path, tok);
            }
        }
        tok = strtok(NULL, "/");
        i++;
    }
    char* temp = (char*)calloc(strlen(*host) + 1, 1);
    strcat(temp, *host);
    tok = strtok(temp, ":");

    // remove port from host pointer
    *host = (char*)realloc(*host, strlen(tok) + 1);
    bzero(*host, sizeof(*host));
    strcat(*host, tok);
    
    // insert port into port pointer
    tok = strtok(NULL, ":");
    if(tok != NULL)
    {
        *port = (char*)calloc(strlen(tok) + 1, 1);
        strcat(*port, tok);
    }
    else
    {
        *port = (char*)calloc(strlen("80") + 1, 1);
        strcat(*port, "80");
    }

    // handle empty path
    if(strlen(*path) == 0)
    {
        *path = (char*)realloc(*path, strlen("/") +1);
        strcat(*path, "/");
    }
}

int main(int argc, char* argv[])
{
    // declare a new socket
    int sock; 
    struct sockaddr_in cli_name; 

    // declare new hostname and host address
    struct addrinfo* res;
    char* hostname;
    char* port;
    char* hostaddr;
    struct sockaddr_in* saddr;

    // declare path ad protocol
    char* path;
    char* protocol;

    // initialize the socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror ("Error opening channel");
        close(sock);
        exit(1);
    }

    // set all bytes of cli_name struct to zeros ("reset the struct")
    bzero(&cli_name, sizeof(cli_name)); 
    // set IP address type to IPv4
    cli_name.sin_family = AF_INET;
    
    // get URL and port from the terminal
    parseCMD(&hostname, &path, &port, &protocol, argc, argv);
    if(strcmp(protocol, "http") != 0)
    {
        printf("Given protocol is not HTTP");
        exit(1);
    }
    // insert URL into the struct
    if (0 != getaddrinfo(hostname, NULL, NULL, &res))
    {
        fprintf(stderr, "Error in resolving hostname %s\n", hostname);
        exit(1);
    }
    saddr = (struct sockaddr_in*)res->ai_addr;
    hostaddr = inet_ntoa(saddr->sin_addr);
    cli_name.sin_addr.s_addr = inet_addr(hostaddr);
    
    // set port number
    char* ptr;
    cli_name.sin_port = htons((in_port_t)strtol(port, &ptr, 10));

    // connect the socket to the address specified by cli_name.
    if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    {
        perror ("Error establishing communications");
        close(sock);
        exit(1);
    }

    // send HTTP request
    char buf[256] = "";
    sprintf(buf, "GET %s HTTP/1.0\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", path, hostname);
    printf("%s\n", buf);
    if(send(sock, buf, strlen(buf), 0) < 0)
    {
        perror("Error sending HTTP request");
        close(sock);
        exit(1);
    }
    // receive incoming HTTP responses
    int bytesReceived = 0;
    while(1)
    {
        bzero(buf,sizeof(buf));
        bytesReceived = recv(sock, buf, sizeof(buf), 0);
        if(bytesReceived < 0)
        {
            perror("Error receiving HTTP response");
            close(sock);
            exit(1);
        }
        if (bytesReceived == 0)
        {
            close(sock);
            break;
        }
        // printf("%s", buf);
        fprintf(stdout, buf);
    }
    printf("\n");
    free(hostname);
    free(port);
    free(path);
    free(protocol);
    return 0;
}