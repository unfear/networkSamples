#include <sys/socket.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <signal.h>
#include <pthread.h>

#define false 0
#define true 1

static int ctrlCSig = false;
static int countThreadCalls = 0;

void sigCatcher(int sig)
{
    ctrlCSig = true;
    printf("quit\n");
}

void* childThreadFunc(void* arg)
{
    do
    {
	printf("Waiting for msg from client...\n");
	countThreadCalls++;
	sleep(5);
    } while(ctrlCSig != true);
}

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
    
    // catch signals
    struct sigaction act;
    memset(&act, 0x0, sizeof(act));
    act.sa_handler = sigCatcher;
    sigaction(SIGINT, &act, 0);

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
	    if(ctrlCSig == true)
	    {
	      bzero(buffer, sizeof(buffer));
	      strcpy(buffer, "quit");
	    }
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
	
	// create thread
	pthread_t pchild;
	if(pthread_create(&pchild, 0, childThreadFunc, 0) != 0)
	{
	    printf("Thread create Error!\n");
	}
	
	int bytes = 0;
	do
	{
	    int addr_len=sizeof(dest);
	    bzero(buffer, sizeof(buffer));
	    bytes = recvfrom(sd, buffer, sizeof(buffer), 0, &dest, &addr_len);
	    printf("msg from client: %s:(%d bytes)\n", buffer, bytes);
	    printf("countThreadCalls: %d\n", countThreadCalls);
	}while( bytes > 0 && strcmp(buffer, "quit") != 0);
	ctrlCSig = true;
	pthread_join(pchild, 0);
    }

    if(close(sd) != 0)
        printf("Error Socket close connection\n");
    
    return 0;
}
