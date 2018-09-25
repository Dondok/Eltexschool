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
	message_buf  rbuf;
	int msg_id;
	
	key = ftok("key",'A'); // создание ключа для очереди
	
	msg_id = msgget(key, 0 ); // присоединение к очереди
	
	if (msg_id == -1){
		printf("клиетн 2 не присоединились к очереди");
	}
	
	/*rbuf.mtype = 10L;
	msgrcv(msg_id2,&rbuf, sizeof(rbuf),10L,0);
	printf("%s",rbuf.mtext);*/

	while(1){
		msgrcv(msg_id,&rbuf, sizeof(rbuf),10L,0);//изъятие из очереди
		printf("%s",rbuf.mtext);
		
		fgets(rbuf.mtext, 128, stdin);
		msgsnd(msg_id, &rbuf, sizeof(rbuf), 0);//посылка сообщ в очередь
		fgets(rbuf.mtext, 128, stdin);
		msgsnd(msg_id, &rbuf, sizeof(rbuf), 0);//посылка сообщ в очередь
		
	}
	
	return 0;
}
