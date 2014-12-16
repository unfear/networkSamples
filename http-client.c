#include <sys/socket.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>

#include <memory.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
        printf("Incorrect input\n");
    else
        printf("Serv: %s, data: %s\n", argv[1], argv[2]);

    int port = 80;
    struct sockaddr_in dest;

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket descriptor: %d\n", sd);

    memset(&dest, 0x0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if(inet_aton(argv[1], &dest.sin_addr.s_addr) == 0)
    {
        printf("Error inet_addr\n");
        return 1;
    }

    if(connect(sd, (struct sockaddr *)&dest, sizeof(dest)) != 0)
    {
        printf("Error Socket connection\n");
        close(sd);
        return 1;
    }
    
    char buf[100];
    memset(buf, 0, sizeof(buf));

    sprintf(buf, "GET %s HTTP/1.0\n\n", argv[2]);
    send(sd, buf, strlen(buf), 0);

    int bytes_read = 0;
    do
    {
        bytes_read = recv(sd, buf, sizeof(buf)-1, 0);
        buf[bytes_read] = 0;
        if(bytes_read > 0)
            printf("%s", buf);

    } while(bytes_read > 0);

    printf("\n");

    if(close(sd) != 0)
        printf("Error Socket close connection\n");

    return 0;
}
