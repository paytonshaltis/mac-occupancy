#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main (int argc, char **argv) {
    int listenfd, connfd, n;
    socklen_t clilen;
    char buf[MAXLINE];
    struct sockaddr_in cliaddr, servaddr;
        
    //creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
        
    //preparation of the socket address 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
        
    bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
        
    listen(listenfd, LISTENQ);
        
    printf("%s\n","Server running...waiting for connections.");
        
    for (;;) {
            
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        printf("%s\n","Received request...");
           
        // clear the buffer before receiving the first message
      	memset(buf, 0, MAXLINE);
                    
        while ((n = recv(connfd, buf, MAXLINE, 0)) > 1)  {
            printf("%s","String received from and resent to the client: ");
            puts(buf);
            send(connfd, buf, n, 0);

            // clear the buffer after receiving each message
            memset(buf, 0, MAXLINE);
        }
                
        if (n < 0) {
            perror("Read error"); 
            exit(1);
        }
        close(connfd);
        printf("%s", "Closed the connection with the client.\n");
    }
    //close listening socket
    close(listenfd); 
}
