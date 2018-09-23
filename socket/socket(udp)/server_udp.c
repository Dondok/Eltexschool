#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


char mess_serv[] = "\nHello client!";
typedef struct sockaddr_in sockaddr_t;
int main()
{
	int sock;
	struct sockaddr_in server,client;
	char buf[1024];
	int bytes_read;

	unsigned int len = sizeof(sockaddr_t);
	
	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	
	memset(&server, 0, sizeof(sockaddr_t));
	
	//заполнение структуры
	server.sin_family = AF_INET;
	server.sin_port = htons(3425);//порт
	// INADDR_ANY - соединения с клиентами через любой IP
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//bind - для явного связывание с некоторым адресом
	if (bind(sock, (struct sockaddr *)&server, len) < 0) {
        perror("bind");
        exit(2);
    }
	
	while(1) {
		
		if((bytes_read = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&client, &len)) > 0){
			printf("%s",buf);
			strcat(buf, mess_serv);
			printf("%s",buf);
		}
		
		if ((sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&client, len)) == -1){
			perror("sendto()");
		}
		bytes_read = -1;
		
	}
		

	return 0;
}
