#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>

typedef struct msgbuf {
    long    mtype;
    char    mtext[128];
} message_buf;

int main()
{
	key_t key;
	message_buf rbuf;
	int msg;
	
	key = ftok("key",'A'); // создание ключа для очереди
	
	msg = msgget(key,IPC_CREAT| 0666); //создание очереди сообщений
	
	if (msg != -1){
		printf("очередь создана\n");
	}
	rbuf.mtype = 10L;//приоритет
	while(1){
		fgets(rbuf.mtext, 128, stdin);
		//printf("%s",rbuf.mtext);
		msgsnd(msg, &rbuf, sizeof(rbuf), 0);
		
		msgrcv(msg,&rbuf, sizeof(rbuf),10L,0);//изъятие из очереди
		printf("%s",rbuf.mtext);
		msgrcv(msg,&rbuf, sizeof(rbuf),10L,0);//изъятие из очереди
		printf("%s",rbuf.mtext);
	}
	
	
	return 0;
}
