#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header_file.h"

	char msg1[] = "Hello server!";
int main()
{
	int sock;
	char buf[1024];
	struct sockaddr_in addr;
//	int bytes_read;
	unsigned int len = sizeof(addr);
	//создание сокета
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	
	//заполнение структуры
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_server);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int count = 0;
	int max_msg = 40;
	while(count < max_msg) {
		count++;
		sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&addr, sizeof(addr));
		
		 if (!fork()) {
            // Принятие сообщения от сервера
            if (recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &len) == -1) {
                perror("Error from recvfrom()");
                exit(EXIT_FAILURE);
            }
            printf("Request %i handled: %s\n", count, buf);
            close(sock);
            exit(EXIT_SUCCESS);
        }
	}
	
	close(sock);
		
	return 0;
}
