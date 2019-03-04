#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

char mess_serv[] = "\nHello client!";
char buf[1024];

typedef struct sockaddr_in sockaddr_t;

void udp_server	(struct sockaddr_in udp,	int udp_fd_sock)
{
	
	struct sockaddr_in  client;	
	unsigned int len = sizeof (client);
	int bytes_read;
	while(1) {
		
		bytes_read = -1;
		
		if((bytes_read = recvfrom(udp_fd_sock, buf, 1024, 0,
		   (struct sockaddr *)&client, &len)) > 0) {
			strcat(buf, mess_serv);
		}
		
		if ((sendto(udp_fd_sock, buf, sizeof(buf), 0,
			(struct sockaddr *)&client, len)) == -1) {
				
			perror("sendto()");
		}
		
		if (bytes_read > 0) break;
	}
	
}


void tcp_server (struct sockaddr_in tcp, int tcp_fd_sock)
{
	int bytes_read;
	int fd_sock;
	listen(tcp_fd_sock, 1);
				while(1) {
					fd_sock = accept(tcp_fd_sock, NULL, NULL);
					
					if(fd_sock < 0) {
						perror("accept");
						exit(3);
					}
					
					while(1) {
					
						bytes_read = recv(fd_sock, buf, 1024, 0);
						if(bytes_read <= 0) break;
						strcat(buf, mess_serv);
						
						send(fd_sock, buf, 2*bytes_read, 0);
						if(bytes_read > 0) break;
					}
					
					close(fd_sock);
					if(bytes_read > 0) break;
				}
				
				
}


int main()
{	
	// для хранения количества описателей возвращаемых select
	int count;
	fd_set fds;
	//  для хранения fd сокетов
	int udp_fd_sock, tcp_fd_sock;
	// для хранения максимального fd сокета
	int max_fd;
	//структура времени ожидани
	struct timeval tv;
	
	struct sockaddr_in tcp, udp;

	tcp_fd_sock = socket(AF_INET, SOCK_STREAM, 0); //создание сокета
	udp_fd_sock = socket(AF_INET, SOCK_DGRAM, 0);
	//проверка на создание сокета, если listeer -1 то не создался сокет
	if(tcp_fd_sock < 0) {
		perror("socket");
		exit(1);
	}
	if(udp_fd_sock < 0) {
		perror("socket");
		exit(1);
	}
	/*заполнение сткутуры */
	//tcp
	tcp.sin_family = AF_INET;
	tcp.sin_port = htons(1111);
	tcp.sin_addr.s_addr = htonl(INADDR_ANY); 
	//udp
	udp.sin_family = AF_INET;
	udp.sin_port = htons(2222);
	udp.sin_addr.s_addr = htonl(INADDR_ANY); 
	
	//опередление максимального fd
	if (udp_fd_sock > tcp_fd_sock){
		max_fd = udp_fd_sock;
	}	else {
		max_fd = tcp_fd_sock;
	}
	
	if ((bind(udp_fd_sock, (struct sockaddr *)&udp, sizeof(udp)) < 0)) {
		perror("bind");
		exit(2);
	}
	if ((bind(tcp_fd_sock, (struct sockaddr *)&tcp, sizeof(tcp))) < 0) {
		perror("bind");
		exit(2);
	}
	
	//ожидание не больше 30 секунд
	tv.tv_sec  = 30;
	tv.tv_usec = 0;
	
	while (1) {
		
		//определяем макросы
		FD_ZERO (&fds);
		FD_SET  (udp_fd_sock, &fds);
		FD_SET  (tcp_fd_sock, &fds);
		
		
		
		count = select(max_fd+1, &fds, NULL, NULL, &tv);
		if (count < 0){
			perror("select");
		}
		for (int i = 0; i < 2; i++){
			if (FD_ISSET(tcp_fd_sock, &fds)){
				tcp_server(tcp, tcp_fd_sock);
			}
			if (FD_ISSET(udp_fd_sock, &fds)){
				
				udp_server(udp, udp_fd_sock);
				
			}
		}
	}
	
	return 0;
}
