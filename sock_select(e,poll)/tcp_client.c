#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

	char message[] = "Hello server!";
	char buf[sizeof(message)];
	
int main()
{
	int sock_id;
	struct sockaddr_in addr;
	
		/*(AF_INET(internet-домен),SOCK_STREAM(для TCP))*/
	sock_id = socket(AF_INET, SOCK_STREAM, 0);//создаем сокет
	
	//проверка на создание сокета
	if(sock_id < 0)
	{
		perror("socket");
		exit(1);
	}
	
	/*заполение структуры*/
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1111);
	
	/* адрес интерфейса внутренней петли(INADDR_LOOPBACK)
	 * Пакеты, направляемые по этому адресу, в сеть не попадают.
	 * Вместо этого они передаются
	 * стеку протоколов TCP/IP как только что принятые.
	 */
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	//на стороне клиента connect - для установление соединения
	if(connect(sock_id, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect");
		exit(2);
	}
	//отправка сообщения
	send(sock_id, message, sizeof(message), 0);
	//принятие сообщения
	recv(sock_id, buf, 2*sizeof(message), 0);
	printf("%s	",buf);
	close(sock_id);
	
	return 0;
}
