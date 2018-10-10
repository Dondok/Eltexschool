#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header_file.h"
//#include <netinet/if_ether.h>

/*
 * задача клиента состоит в следующем:
 * клиент формирует RAW сокет UDP и посредством "сырого" сокета отправ-
 * ляет на сервер строку, сервер добавляет к стороке свою строку и от-
 * правляем обратно клиенту. клиент должен это принять и обработать и
 * вывести.
 */
char msg1[] = "Hello server!";
char msgbuf[buf_size];
char *msgbuf2;
char receive[packet_size];
//структура заголовка UDP

//функция сравнения назначения порта с портом клиента.
int is_server_packet(struct sockaddr_in server, struct UdpHeader input);

int main()
{
	int sock;
	int bytes_read;
	struct UdpHeader header;
	unsigned int len = sizeof(struct sockaddr_in);
	
	//создание сокета
	sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	
	//зануление структуры сокета сервера
	memset(&server, 0, sizeof(struct sockaddr_in));
	//заполнения структуры сервера
    server.sin_family = AF_INET;
    //петля
    server.sin_addr.s_addr = inet_addr("127.0.0.3");
    /*
     * не имеет смысла задавать номер порта, потому что он в
     * заголовке UDP
	 */
    //server.sin_port = htons(3333);
    
    //зануление структуры сокета клиента
    memset(&client, 0, sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons(port_client);
    
    //заполнение заголовка UDP
    header.source_port = htons(port_client);
    header.dest_port = htons(port_server);
	header.length = htons(sizeof(header)+sizeof(msg1));
	header.checksum = 0;
    
    //заполнение буфера с заголовками для отправки.
    memcpy(msgbuf, &header, sizeof(header));
	memcpy((void *)(msgbuf+sizeof(header)), (void *)msg1, sizeof(msg1));
    
    //printf("%s", msgbuf);
    
	if ((sendto(sock, msgbuf, sizeof(header)+sizeof(msg1), 0,
		(struct sockaddr *)&server, sizeof(server))) == -1){
			perror("sendto()");
		}
	
	struct UdpHeader *sender_pack;
	
	while(1) {
		 memset(receive, 0, packet_size);
		
		if((bytes_read = recvfrom(sock, receive, 2*sizeof(receive), 0,
			(struct sockaddr *)&server, &len)) > 0){
		}
		
		sender_pack = (struct UdpHeader *) &(receive[IP_head_len]);
		
		if((is_server_packet(server, *sender_pack)) == 1){
			msgbuf2 = (char *) &(receive[IP_head_len+UDP_head_len]);
			printf ("%s\n", msgbuf2);
			break;
		}
			
	} 
	
	
	close(sock);
		
	return 0;
}
		
int is_server_packet(struct sockaddr_in server, struct UdpHeader input)
{
	if ((input.dest_port == htons(port_client)))
		return 1;
}
   		

