#include <sys/socket.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int main(int argc, char * argv[])
{
    int port = 9999;
    struct sockaddr_in dest;
    char buffer[1024];

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket descriptor: %d\n", sd);

    memset(&dest, 0x0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    dest.sin_addr.s_addr = INADDR_ANY;
    if ( bind(sd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
	printf("Error Socket bind\n");
	close(sd);
	return 1;
    }
    if( listen(sd, 20) != 0)
    {
	printf("Error Socket listen\n");
	close(sd);
	return 1;
    }

    do
    {
	memset(&buffer, 0x0, sizeof(buffer));
	int clientSd;
	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);
	clientSd = accept(sd, (struct sockaddr*)&client_addr, &addrlen);
	send(clientSd, buffer, recv(clientSd, buffer, 1024, 0), 0);
	printf("Recieved message: %s\n", buffer);
	close(clientSd);
    }while( strcmp("bye", buffer) != 0);

    if(close(sd) != 0)
        printf("Error Socket close connection\n");

    return 0;
}
