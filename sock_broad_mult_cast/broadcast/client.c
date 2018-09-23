#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char buf[1024];

typedef struct sockaddr_in sockaddr_t;
int main()
{
	int fd_sock;
	struct sockaddr_in udp, server;
	
	unsigned int len = sizeof (server);
	
	//udp
	udp.sin_family = AF_INET;
	udp.sin_port = htons(2222);
	udp.sin_addr.s_addr = inet_addr("255.255.255.255");
	
	fd_sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if ((bind(fd_sock, (struct sockaddr *)&udp, sizeof(udp)) < 0)) {
		perror("bind");
		exit(2);
	}
	
	if ((recvfrom(fd_sock, buf, 1024, 0, (struct sockaddr *)&server, &len)) < 0){
		perror("recvfrom");
	}
	
	printf("%s",buf);
	
	

	
	return 0;
}
