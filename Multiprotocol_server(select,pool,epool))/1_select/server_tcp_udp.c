// Реализация мультипротокольного сервера

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#include "header_file.h"

char mess_serv[] = "\nHello client!";

int main()
{
	int sock_udp, sock_tcp;
	struct sockaddr_in server;
	struct sockaddr_in client;
	fd_set fds;
	// для хранения максимального fd сокета
	int fd_max;
	//unsigned int len = sizeof(struct sockaddr_in);
	socklen_t len2 = sizeof(client);
	char buf[256];
	int accept_tcp;
	
	//создание сокетов
	if ((sock_tcp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error TCP");
        exit(EXIT_FAILURE);
    }
    if ((sock_udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket error UDP");
        exit(EXIT_FAILURE);
    }
    
    //заполнение структур описывающих tcp/udp сервер
    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port_server);
	
	  // Привязка к адресу
    if (bind(sock_udp, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error UDP");
        exit(EXIT_FAILURE);
    }
    if (bind(sock_tcp, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("bind error TCP");
        exit(EXIT_FAILURE);
    }
    
    listen(sock_tcp, 1);
    int count = 0; //для подчета количества притяных сообщений(TCP/UDP)
	while(1){
	
		fd_max = (sock_tcp > sock_udp) ? sock_tcp + 1 : sock_udp + 1;
		
		//определяем макросы
		FD_ZERO(&fds);
		FD_SET(sock_tcp, &fds);
		FD_SET(sock_udp, &fds);
		
		if (select(fd_max, &fds, NULL, NULL, NULL) == -1) {
			perror("select error");
			exit(EXIT_FAILURE);
		}
				
		if (FD_ISSET(sock_udp, &fds)){
			if((recvfrom(sock_udp, buf, 256, 0,
				(struct sockaddr *)&client, &len2)) == -1){	
				perror("recvfrom UDP()");
				exit(EXIT_FAILURE);
			}

			strcat(buf, mess_serv);
		
			if ((sendto(sock_udp, buf, sizeof(buf), 0, 
				(struct sockaddr *)&client, len2)) == -1){
				perror("sendto UDP");
				exit(EXIT_FAILURE);
			}
			count++;
			printf("packet UDP(%i)\n",count);
		}
		
		if (FD_ISSET(sock_tcp, &fds)){
			if ((accept_tcp = accept(sock_tcp, 
				(struct sockaddr *) &client,&len2)) < 0) {
                perror("accept error");
                exit(EXIT_FAILURE);
            }
			if (recv(accept_tcp, buf, 256, 0) == -1) {
                perror("recv error");
                exit(EXIT_FAILURE);
            }

			strcat(buf, mess_serv);
			
			if (send(accept_tcp, buf, 256, 0) == -1) {
                perror("send error");
                exit(EXIT_FAILURE);
            }
			close(accept_tcp);
			count++;
			printf("packet TCP(%i)\n",count);
			
		}
	
	
	}
    
    
	close(sock_udp);
	close(sock_tcp);
	return 0;
}
