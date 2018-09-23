#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>

typedef struct msgbuf {
    long    mtype;
    char    mtext[256];
} message_buf;


/* функция функция для принятие сообщения и вывод на экран*/
void *get_msg(void *ptr)
{
	key_t key;
	message_buf  rbuf;
	int msg_id;
	int id;
	
	key = ftok("key_msg",'A'); // создание ключа для очереди
	msg_id = msgget(key, IPC_CREAT| 0666 ); // присоединение к очереди
	while(1){
		if((id = msgrcv(msg_id,&rbuf,sizeof(rbuf),3L,0)) > 0){
		
			printf ("%s\n",rbuf.mtext);
	
		}
	}
	return 0;
}

/* функция для отправки сообщения*/
void *send_msg(void *ptr)
{
	key_t key;
	message_buf  rbuf;
	char text[256];
	int msg_id;
	key = ftok("key_msg",'A'); // создание ключа для очереди
	msg_id = msgget(key, IPC_CREAT| 0666 ); // присоединение к очереди
	rbuf.mtype = 2L; //приоритет для отправление сообщений
	while(1){
		
		fgets(text, 256, stdin);
		sprintf(rbuf.mtext,"%d:%s",getpid(),text);
		msgsnd(msg_id, &rbuf, sizeof(rbuf), 0);
		
	}
	return 0;
}





int main()
{
	key_t key;
	pthread_t get, send;
	message_buf  rbuf;
	int msg_id;
	key = ftok("key_msg",'A'); // создание ключа для очереди
	msg_id = msgget(key, IPC_CREAT| 0666 ); // присоединение к очереди
	if (msg_id == -1){
		printf("клиент 1 не присоединилися к очереди");
	}
	
	/* посылка pid серверу*/
	rbuf.mtype = 1L; // приоритет для отправки pid
	sprintf(rbuf.mtext,"%d",getpid());
	printf ("посылаем pid - %s серверу \n", rbuf.mtext);
	msgsnd(msg_id, &rbuf, sizeof(rbuf), 0);
	
	
	pthread_create(&get, NULL, get_msg, NULL);
	pthread_create(&send, NULL, send_msg, NULL);
	pthread_join(get, NULL);
	pthread_join(send, NULL);
	
	
	return 0;
}
