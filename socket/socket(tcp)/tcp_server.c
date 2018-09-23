#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

char mess_serv[] = "\nHello client!";
int main()
{
	/*
	 * listener для хранения fd сокета
	 */
	int fd_sock, listener;
	struct sockaddr_in addr;
	char buf[1024];
	int bytes_read;
/*(AF_INET(internet-домен),SOCK_STREAM(для TCP),протокол по умолчанию)*/
	listener = socket(AF_INET, SOCK_STREAM, 0); //создание сокета
	
	//проверка на создание сокета, если listeer -1 то не создался сокет
	if(listener < 0) {
		perror("socket");
		exit(1);
	}
	
	/*заполнение сткутуры*/
	
	addr.sin_family = AF_INET; //семейство адресов
	
	//Host TO Network Short - преобразование числа порядка хоста в сетевой
	addr.sin_port = htons(3425); //номер порта
	
	//Host TO Network Long - реобразование числа порядка хоста в сетевой
	// INADDR_ANY - соединения с клиентами через любой IP
	addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP-адрес
	
	//bind - для явного связывание с некоторым адресом
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		exit(2);
	}
	
	/*очередь запросов на соединение
	 * первый параметр это fd сокета
	 * второй это размер очереди запросов
	 */
	 
	listen(listener, 1);
	while(1) {
		//accept - создает для общение с клиентом новый сокет
		fd_sock = accept(listener, NULL, NULL);
		
		if(fd_sock < 0) {
			perror("accept");
			exit(3);
		}
		
		while(1) {
			
			//recv - принимает данные
			bytes_read = recv(fd_sock, buf, 1024, 0);
			if(bytes_read <= 0) break;
			strcat(buf, mess_serv);
			//send - для отправки данных 
			send(fd_sock, buf, 2*bytes_read, 0);
		}
		
		close(fd_sock);
	}
	return 0;
}
