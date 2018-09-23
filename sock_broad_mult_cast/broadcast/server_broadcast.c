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
	int fd_sock;
	int val = 1;
	struct sockaddr_in udp;
	
	unsigned int len = sizeof (udp);
	
	//udp
	udp.sin_family = AF_INET;
	udp.sin_port = htons(2222);
	udp.sin_addr.s_addr = inet_addr("255.255.255.255");
	
	fd_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	
	if((setsockopt(fd_sock, SOL_SOCKET, SO_BROADCAST, &val,
		sizeof(val))) < 0){
		
		perror("setsockopt");
	}
	
	if( (sendto (fd_sock, mess_serv, sizeof(mess_serv), 0, 
			(struct sockaddr *)&udp, len)) < 0)	{
				
		perror("sendto");
	}
		
	

	
	return 0;
}
