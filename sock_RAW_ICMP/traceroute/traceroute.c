// реализация утилиты traceroute
	
#include <stdlib.h>
#include <stdio.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define ttl_limit 30

struct packet{
	struct iphdr h_ip;
	struct icmphdr h_icmp;
};
	
/*	функция для вычисления контрольной суммы */
unsigned short csum(unsigned short *buf, int nwords);
/*	функция для выделения нужного icmp пакета*/
int recv_packet(struct packet reply, struct packet request);

int main(int argc, char* argv[])
{
	if(argc != 2){
		perror("usage the destination IP address");
		exit(EXIT_SUCCESS);
	}
	
	int sock_raw;
	unsigned int len = sizeof(struct sockaddr_in);
	
	//создание сокета
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sock_raw < 0){
		perror("socket ERR");
		exit(1);
	}
	
	int value = 1;
	int check = 0;
	setsockopt(sock_raw, IPPROTO_IP, IP_HDRINCL, &value, sizeof(value));
	
	// Заполнение структуры, описывающей сервер
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = 12345;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    
    struct packet req;
	struct packet reply;
	struct in_addr ntoa; //for inet_ntoa
	int count = 0; // for ttl
	/*
	 * отправляем пакеты до тех пор пока:
	 *  1. ttl != 30
	 *  2. пакет не дойдет до назначенного адреса
	 * 	3. конечный адрес достижим
	*/
	do{
		
		count++;
		memset(&req, 0, sizeof(req));
		//h_ip
		req.h_ip.version = 4;
		req.h_ip.ihl = 5;
		req.h_ip.ttl = count;
		req.h_ip.protocol = IPPROTO_ICMP;
		req.h_ip.saddr = inet_addr("192.168.1.2");
		req.h_ip.daddr = inet_addr(argv[1]);
		//h_icmp
		req.h_icmp.type = ICMP_ECHO;
		req.h_icmp.code = 0;
		req.h_icmp.un.echo.id = rand() % 65000;
		req.h_icmp.checksum = csum((unsigned short *)&req.h_icmp,
		sizeof(struct icmphdr)/2);
		
		//последовательная отправка пакета с увеличенным ttl.
		if ((sendto(sock_raw, &req, sizeof(req), 0, 
					(struct sockaddr *)&saddr,	len) == -1)){
				perror("sendto ERR");
				exit (1);		
		}	

		//принятие пакета тех пор пока не придет нужный пакет
		do{
			memset(&reply, 0, sizeof(reply));
			if(recvfrom(sock_raw, &reply, sizeof(reply),0,
				(struct sockaddr *)&saddr, &len) < 0){
				perror("recvfron ERR");
				exit (1);
			}
		} while((check = recv_packet(reply,req)) == 0);
	
		ntoa.s_addr = reply.h_ip.saddr;
		printf("%i %s \n", count ,inet_ntoa(ntoa));
		
		if(check == 2){
			printf("Destination Unreachable");
			break;
		}
		
		sleep(1);
	} while((req.h_ip.ttl != ttl_limit)&&(reply.h_ip.saddr != req.h_ip.daddr));
	
	close(sock_raw);
	return 0;
}

int recv_packet(struct packet reply, struct packet request)
{
	
	if((reply.h_ip.protocol == IPPROTO_ICMP)
		&&(reply.h_ip.daddr == request.h_ip.saddr)){
		
		switch (reply.h_icmp.type) {
			case ICMP_ECHOREPLY:
			case ICMP_TIME_EXCEEDED:
				return 1;
			case ICMP_DEST_UNREACH:
				return 2;
        }
	}
	return 0;
}

unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
