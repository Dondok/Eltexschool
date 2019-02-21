#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <sys/ioctl.h> 
#include <net/if.h> 

#include "header_file.h"


/*
 * задача клиента состоит в следующем:
 * клиент посредством RAW сокета формирует пакет и отправляет сообщение
 * серверу который находится на другом ПК, сервер добавляет к принятому 
 * сообщению строку и отправляет его обратно клиенту.
 */ 

	char msg1[256] = "Hello server!";
	char packet[packet_size];
	char msgbuf[h_all_p_len + sizeof(msg1)];
	char *msgbuf1;			
/*
 * функция сравнения порта источка с портом назначения, чтобы определить
 * пакет назначенный клиенту.
 */
int is_server_packet(struct udphdr input);

/*	функция для вычисления контрольной суммы */
unsigned short csum(unsigned short *buf, int nwords);


int main()
{
	int sock_raw;
	int bytes_read = -1;
	unsigned int len = sizeof(struct sockaddr_ll);
	
	//создание сокета
	sock_raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_raw < 0){
		perror("socket ERR");
		exit(1);
	}
		
		
	// Заполнение структуры, описывающей сервер
	struct sockaddr_ll server;
	memset(&server, 0, sizeof(struct sockaddr_ll));
	server.sll_family   = AF_PACKET;
	server.sll_ifindex  = if_nametoindex("wlp3s0");
	server.sll_halen    = ETH_ALEN;
	server.sll_protocol = htons(ETH_P_ALL);
	server.sll_addr[0] 	= MY_DEST_MAC0;
	server.sll_addr[1] 	= MY_DEST_MAC1;
	server.sll_addr[2] 	= MY_DEST_MAC2;
	server.sll_addr[3] 	= MY_DEST_MAC3;
	server.sll_addr[4] 	= MY_DEST_MAC4;
	server.sll_addr[5]  = MY_DEST_MAC5;
    
    
	/*
	* заполнение заголовка UDP
	*/		
	struct udphdr h_udp;
	memset(&h_udp, 0, sizeof(struct udphdr));				
	h_udp.source = htons(port_client);
	h_udp.dest = htons(port_server);
	h_udp.len = htons(h_udp_len + sizeof(msg1));
	h_udp.check = 0;
    
    
	/*
	* заполнение заголовка IP
	*/
	struct iphdr h_ip;
	memset(&h_ip, 0, sizeof(struct iphdr));
	h_ip.version = 4;
	h_ip.ihl = 5;
	h_ip.tos = 0;
	h_ip.tot_len = htons(h_ip_len + h_udp_len + sizeof(msg1));
	h_ip.id = htons(12345);
	h_ip.frag_off = 0;
	h_ip.ttl = 64; 
	h_ip.protocol = IPPROTO_UDP; 
	h_ip.saddr = inet_addr("192.168.1.4");
	h_ip.daddr = inet_addr("192.168.1.9");
	h_ip.check = csum((unsigned short *) &h_ip, sizeof(struct iphdr)/2);


	/*
	* заполнение заголовка канального уровня
	*/
	struct ethhdr h_eth;
	memset(&h_eth, 0, sizeof(struct ethhdr));
	h_eth.h_dest[0] = MY_DEST_MAC0;
	h_eth.h_dest[1] = MY_DEST_MAC1;
	h_eth.h_dest[2] = MY_DEST_MAC2;
	h_eth.h_dest[3] = MY_DEST_MAC3;
	h_eth.h_dest[4] = MY_DEST_MAC4;
	h_eth.h_dest[5] = MY_DEST_MAC5;
	h_eth.h_source[0] = MY_SOURCE_MAC0;
	h_eth.h_source[1] = MY_SOURCE_MAC1;
	h_eth.h_source[2] = MY_SOURCE_MAC2;
	h_eth.h_source[3] = MY_SOURCE_MAC3;
	h_eth.h_source[4] = MY_SOURCE_MAC4;
	h_eth.h_source[5] = MY_SOURCE_MAC5;	
	h_eth.h_proto	 = htons(ETH_P_IP);
  
	//заполнение буфера с заголовками для отправки.
	memcpy(msgbuf, &h_eth, h_eth_len);
	memcpy((void *)(msgbuf + h_eth_len), &h_ip, h_ip_len);
	memcpy((void *)(msgbuf + h_eth_len + h_ip_len),	&h_udp, h_udp_len);
	memcpy((void *)(msgbuf + h_all_p_len), &msg1, sizeof(msg1));
	
	/*
	* отправление сформированного пакета серверу
	*/
	if ((sendto(sock_raw, &msgbuf, sizeof(msgbuf), 0, 
			(struct sockaddr *)&server,	len) == -1)){
		perror("sendto ERR");
		exit (1);		
	}
	
	struct udphdr *recv_packet;
	while(1) {	
		memset(packet, 0, packet_size);
		
		if((bytes_read = recvfrom(sock_raw, packet, 2*sizeof(packet), 0,
			(struct sockaddr *)&server, &len)) > 0){
		}
		
		recv_packet = (struct udphdr *) &(packet[h_eth_len + h_ip_len]);
		
		if((is_server_packet(*recv_packet)) == 1){
			msgbuf1 = (char *) &(packet[h_all_p_len]);
			printf ("%s\n", msgbuf1);
			break;
		}
	}
	
	close(sock_raw);
	return 0;
}


int is_server_packet(struct udphdr input){
	int equal = 0;
	if ((ntohs(input.dest) == port_client)){
		equal = 1;
	}
	return equal;
}

unsigned short csum(unsigned short *buf, int nwords)
{
	unsigned long sum;
	for(sum=0; nwords>0; nwords--){
		sum += *buf++;
	}
	sum = (sum >> 16) + (sum &0xffff);
	sum += (sum >> 16);
	return (unsigned short)(~sum);
}

