
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "header_file.h"


/*
 * задача клиента состоит в следующем:
 * клиент посредством RAW сокета формирует пакет и отправляет сообщение
 * серверу, сервер добавляет к принятому сообщению строку и отправляет
 * его обратно клиенту.
 * Нужно заполнить загловки транспортного"UDP" и сетевого"IP" уровня.
 */
 
char msg1[] = "Hello server!";
char *msgbuf;

/*
 * функция сравнения порта источка с портом назначения, чтобы отделить
 * отделить пакеты не назначенные клиенту.
 */
int is_server_packet(struct sockaddr_in server, struct UdpHeader input);

struct sockaddr_in server,client;

int main()
{
	int sock;
	int bytes_read;
	int val = 1;
	char packet[packet_size];
	    
	unsigned int len = sizeof(struct sockaddr_in);
	
	//создание сокета
	sock = socket(AF_INET, SOCK_RAW/* = 3*/, IPPROTO_UDP);
	if(sock < 0){
		perror("socket ERR");
		exit(1);
	}
	
	/* 
	 *yказываем, что IP заголовок бyдет создан нами, иначе ядpо добавит
	 * IP заголовок надо объяснить, что это законстанты IPPROTO_IP,
	 * IP_HDRINCL, зачем взводится val и т.д.
	 */
	if (setsockopt (sock, IPPROTO_IP, IP_HDRINCL, &val,
													sizeof(val))== -1){
		perror ("setsockopt ERR");
		exit (1);
	}
	
	//заполнения структуры сервера
	memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(port_server);
    
    //заполнения структуры клиента
    memset(&client, 0, sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("127.0.0.1");
    client.sin_port = htons(port_client);
    
    //заполнение заголовка UDP
    struct UdpHeader *send_udp 
						= (struct UdpHeader *)&(packet[IP_head_len]);
						
    memset(send_udp, 0, sizeof(struct UdpHeader));
    send_udp->source_port = htons(port_client);
    send_udp->dest_port = htons(port_server);
	send_udp->length = htons(UDP_head_len + buf_size);
	send_udp->checksum = 0;
    
	//заполнение заголовка IP
	struct IP_Header *send_ip = (struct IP_Header *) &(packet);
	memset(send_ip, 0, sizeof(struct IP_Header));
    send_ip->ver_head_len = 69;
    send_ip->ip_tos = 0; //приоритет пакета от 0 до 7
    send_ip->total_len =TOTAL_len;
    send_ip->ip_id = 1000;
    send_ip->ip_off = 64;
    /* 
     * ttl после прохождения маршрутизатора уменьшается на единицу
     * максимум это 255 маршрутизаторов. дальше не пойдет.
     */
    send_ip->ip_ttl = 255; 
    /*
     * код протокола верхрнего уровня, 17 = UDP
     */
    send_ip->ip_type_prot = 17; 
    send_ip->ip_check_sum = 0;
	send_ip->ip_src = inet_addr("127.0.0.1");
	send_ip->ip_dest = inet_addr("127.0.0.1");
    
    //доавление полезной нагрузки
	strcpy(&(packet[IP_head_len + UDP_head_len]), msg1);

	/*
	 * отправление сформированного пакета серверу
	 */
	if ((sendto(sock, packet, packet_size,0,(struct sockaddr *)&server,
		len) == -1)){
		perror("sendto ERR");
		exit (1);		
	}
	
	struct UdpHeader *sender_pack;
	
	while(1) {
		
		 memset(packet, 0, packet_size);
		
		if((bytes_read = recvfrom(sock, packet, 2*sizeof(packet), 0,
			(struct sockaddr *)&server, &len)) > 0){
		}
		
		sender_pack = (struct UdpHeader *) &(packet[IP_head_len]);
		
		if((is_server_packet(server, *sender_pack)) == 1){
			msgbuf = (char *) &(packet[IP_head_len+UDP_head_len]);
			printf ("%s\n", msgbuf);
			break;
		}
			
	} 
	
	
	close(sock);
		
	return 0;
}
int is_server_packet(struct sockaddr_in server, struct UdpHeader input){
	if ((input.dest_port == htons(port_client))){
		return 1;
	}
}

