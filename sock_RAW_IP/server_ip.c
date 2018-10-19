#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header_file.h"

char mess_serv[] = "Hello client!";

struct sockaddr_in server,client;

int main()
{
	int sock;
	char buf[buf_size];
	int bytes_read;

	unsigned int len = sizeof(struct sockaddr_in);
	
	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	
	//заполнения структуры сервера
	memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port_server);
    
	//bind - для явного связывание с некоторым адресом
	if (bind(sock, (struct sockaddr *)&server, len) < 0) {
        perror("bind");
        exit(2);
    }
    	
    /*
     * принимает данные от клиента добавляется к принятому тексту
     * добавляет строку и отправляет обратно клиенту.
     */
	while(1) {
		
		if((bytes_read = recvfrom(sock, buf, 1024, 0,
			(struct sockaddr *)&client, &len)) > 0){
				
			printf("%s\n",buf);
			
		}
		
		strcat(buf, mess_serv);
			
			
		if ((sendto(sock, buf, sizeof(buf), 0, 
			(struct sockaddr *)&client, len)) == -1){
				
			perror("sendto()");
		}
		bytes_read = -1;
		
	}
		

	return 0;
}
