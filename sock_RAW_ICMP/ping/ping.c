// реализация утилиты ping
	
#include <stdlib.h>
#include <stdio.h>
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include  <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netinet/udp.h>
#include <arpa/inet.h>


struct p_reply{
	struct iphdr h_ip;
	struct icmphdr h_icmp;
	char payload[64];
};

/*	функция для вычисления контрольной суммы */
unsigned short csum(unsigned short *buf, int nwords);

int main(int argc, char* argv[])
{
	if(argc != 2){
		perror("usage the destination IP address");
		exit(1);
	}
	
	int sock_raw;
	unsigned int len = sizeof(struct sockaddr_in);
	
	//создание сокета
	sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(sock_raw < 0){
		perror("socket ERR");
		exit(1);
	}
	
	// Заполнение структуры, описывающей сервер
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    saddr.sin_port = 12345;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);
    
	struct icmphdr request;
	struct p_reply reply;
	struct in_addr ntoa; //for inet_ntoa
	
	int count = 0; // for h_icmp.sequence
	while(1){
		
		count++;
		//заполнение заголовка icmp
		memset(&request, 0, sizeof(struct icmphdr));
		request.type = ICMP_ECHO;
		request.code = 0;
		request.un.echo.id = rand() % 65000;
		request.un.echo.sequence = count;
		request.checksum = csum((unsigned short *)&request,
		sizeof(struct icmphdr)/2);
		
		
		if ((sendto(sock_raw, &request, sizeof(request), 0, 
					(struct sockaddr *)&saddr,	len) == -1)){
				perror("sendto ERR");
				exit (1);		
		}	

		memset(&reply, 0, sizeof(reply));
		if(recvfrom(sock_raw, &reply, sizeof(reply),0,
			(struct sockaddr *)&saddr, &len) < 0){
			perror("recvfron ERR");
			exit (1);
		}
		
		//сравнение dest и sousce addr и id для выделения нужного пакета
		if((saddr.sin_addr.s_addr == reply.h_ip.saddr)&&
		(request.un.echo.id == reply.h_icmp.un.echo.id)){	
			
			ntoa.s_addr = reply.h_ip.saddr;
			printf("from %s ", inet_ntoa(ntoa));
			printf("seq %i ", reply.h_icmp.un.echo.sequence);
			printf("ttl %i \n", reply.h_ip.ttl);
			
		}
		
		sleep(1);
	}
	close(sock_raw);
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

