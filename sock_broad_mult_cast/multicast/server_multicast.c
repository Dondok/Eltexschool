#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char mess_serv[] = "\nHello client!";
char buf[1024];

typedef struct sockaddr_in sockaddr_t;
int main()
{
	int fd_sock_udp;

	struct sockaddr_in client;
	
	unsigned int len = sizeof (client);
	
	
	client.sin_family = AF_INET;
	client.sin_port = htons(2222);
	//multicast addr 224.0.0.0 -> 239.255.255.255
	client.sin_addr.s_addr = inet_addr("224.0.0.1");
	
	fd_sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	
	if( (sendto (fd_sock_udp, mess_serv, sizeof(mess_serv), 0,
	 			(struct sockaddr *)&client, len)) < 0)	{
				
		perror("sendto");
	}
		
	

	
	return 0;
}
