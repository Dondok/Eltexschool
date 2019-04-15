#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "header_file.h"

	char message[] = "Hello server!";
	char buf[sizeof(message)];
	
int main()
{
	int sock;
	struct sockaddr_in server;
	int max_msg = 40;
	int count = 0;
	 while (count < max_msg) {
        count++;
        // Создание endpoint'а
        if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
            perror("socket() error");
            printf("%i\n\n", count);
            exit(EXIT_FAILURE);
        }

        // Заполнение структуры, описывающей сервер
        memset(&server, 0, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr("127.0.0.1");
        server.sin_port = htons(port_server);
        // Подключение к серверу
        int res_conn = connect(sock,(struct sockaddr *) &server,
                               sizeof(server));
        if (res_conn == -1) {
            perror("connect() error");
            printf("%i\n\n", count);
            exit(EXIT_FAILURE);
        }
        // Отправка сообщения серверу
        if (send(sock, buf, sizeof(buf), 0) == -1) {
            perror("send() error");
            printf("%i\n\n", count);
            exit(EXIT_FAILURE);
        }
        if (!fork()) {
            // Принятие сообщения от сервера
            if (recv(sock, buf, sizeof(buf), 0) == -1) {
                perror("recv() error");
                exit(EXIT_FAILURE);
            }
            printf("Request %i handled: %s\n", count, buf);
            close(sock);
            exit(EXIT_SUCCESS);
        }
    }
	close(sock);
	
	return 0;
}
