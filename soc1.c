#include <sys/socket.h>
//#include <sys/resolv.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>

#include <memory.h>

int main()
{
    int port = 13;
    struct sockaddr_in dest;
    char * host = "127.0.0.1";
    //char * host = "106.125.34.15";

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    printf("Socket descriptor: %d\n", sd);

    memset(&dest, 0x0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    inet_aton(host, &dest.sin_addr);

    if(connect(sd, (struct sockaddr *)&dest, sizeof(dest)) != 0)
    {
        printf("Error Socket connection\n");
        close(sd);
        return 1;
    }
    
    char buf[] = "Hello";
    send(sd, buf, strlen(buf), 0);

    char newBuf[100];
    memset(newBuf, 0, sizeof(newBuf));
    recv(sd, newBuf, 99, 0);
    printf("Buf :%s\n", newBuf);

    if(close(sd) != 0)
        printf("Error Socket close connection\n");

    return 0;
}
