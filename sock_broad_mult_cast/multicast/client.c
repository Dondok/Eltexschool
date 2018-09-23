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
	int fd_sock_udp;
	struct sockaddr_in client, server;
	struct ip_mreq mreq;
	int val = 1;
	unsigned int len = sizeof (server);
	
	//udp
	client.sin_family = AF_INET;
	client.sin_port = htons(2222);
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	
	fd_sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	
	setsockopt(fd_sock_udp, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    setsockopt(fd_sock_udp, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	
	if ((bind(fd_sock_udp, (struct sockaddr *)&client, sizeof(client)) < 0)) {
		perror("bind");
		exit(2);
	}
	
	
	
	if ((recvfrom(fd_sock_udp, buf, 1024, 0,
		(struct sockaddr *)&server, &len)) < 0){
		perror("recvfrom");
	}
	
	printf("%s",buf);
	
	

	
	return 0;
}
