#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


	char msg1[] = "Hello server!";
int main()
{
	int sock;
	char buf[1024];
	struct sockaddr_in addr;
	int bytes_read;
	//создание сокета
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		perror("socket");
		exit(1);
	}
	
	//заполнение структуры
	addr.sin_family = AF_INET;
	addr.sin_port = htons(3425);
	/* адрес интерфейса внутренней петли(INADDR_LOOPBACK)
	 * Пакеты, направляемые по этому адресу, в сеть не попадают.
	 * Вместо этого они передаются
	 * стеку протоколов TCP/IP как только что принятые.
	 */
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	if ((sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&addr, sizeof(addr))) == -1){
			perror("sendto()");
		}
	
	while(1) {
		
		unsigned int len = sizeof(addr);
		
		if((bytes_read = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, &len)) > 0){
		
			printf("%s",buf);
		}
		if(bytes_read > 0) break;
	}
	
	close(sock);
		
	return 0;
}
