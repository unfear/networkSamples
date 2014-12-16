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
    char * host = "127.0.0.1";
    char buffer[1024];

    int sd = socket(PF_INET, SOCK_DGRAM, 0);
    printf("Socket descriptor: %d\n", sd);

    memset(&dest, 0x0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    char * client = "client";
    
    if(argc == 2 && strcmp(argv[1], client) == 0)
    {
        printf("Program works in Client mode!\n");
	
	inet_aton(host, &dest.sin_addr);
	if(connect(sd, (struct sockaddr *)&dest, sizeof(dest)) != 0)
	{
	    printf("Error Socket connection\n");
	    close(sd);
	    return 1;
	}
	
	int str_len = 0;
	do
	{
	    bzero(buffer, sizeof(buffer));
	    str_len = scanf ("%1023s",buffer);
	    printf("Send to server: %s, len %d\n", buffer, strlen(buffer));
	    send(sd, buffer, strlen(buffer), 0);
	} while(str_len > 0 && strcmp(buffer, "quit") != 0);
    }
    else
    {
	printf("Program works in Server mode!\n");
	
	dest.sin_addr.s_addr = INADDR_ANY;
	if ( bind(sd, &dest, sizeof(dest)) != 0 )
	{
	    printf("Error Socket bind\n");
	    close(sd);
	    return 1;
	}
	
	while (1)
	{
	    int bytes, addr_len=sizeof(dest);

	    bytes = recvfrom(sd, buffer, sizeof(buffer), 0, &dest, &addr_len);
	    printf("msg from client: %s:(%d bytes)\n", buffer, bytes);
	    bzero(buffer, sizeof(buffer));
	}
    }

    if(close(sd) != 0)
        printf("Error Socket close connection\n");

    return 0;
}
