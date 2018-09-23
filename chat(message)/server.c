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

int client[256];
int num = 0;

/*функция для приема и сохранения pid клиента*/
void* pid_client(void *func)
{
	key_t key = ftok("key_msg",'A'); // создание ключа для очереди	
	int msg_id = msgget(key,IPC_CREAT| 0666); //создание очереди сообщений
	message_buf rbuf;
	int id;
	int n;
	
	while((id = msgrcv(msg_id,&rbuf,sizeof(rbuf),1L,0)) != -1){
		
		sscanf(rbuf.mtext,"%d",&n);
		client[num] = n;
		printf("клиент %i подключился!\n",n);
		num++;
	}	
	return 0;
}



int main()
{
	key_t key;
	pthread_t tid;
	
	message_buf rbuf;
	int msg_id;
	key = ftok("key_msg",'A'); // создание ключа для очереди	
	msg_id = msgget(key,IPC_CREAT| 0666); //создание очереди сообщений
	if (msg_id != -1){
		printf("очередь создана\n");
	}
	
	pthread_create(&tid, NULL, pid_client, NULL);
	
	
	/* принимаем и отправляем сообщения всем, кроме отправителя*/
	while (1) {
		if( msgrcv(msg_id,&rbuf,sizeof(rbuf),2L,0) > 0 ) {
			int pid; char str[256];
			/* записываем из строки rbuf.mtext в pid, pid отправителя
			 * а в str сообщение
			 */
			sscanf(rbuf.mtext,"%d:%s",&pid,str);
			printf("%d -> %s\n",pid,str);
			
			for( int i = 0; i < num; i++){
				rbuf.mtype = 3L;
			//	if(client[i] != pid){
					msgsnd(msg_id,&rbuf,sizeof(rbuf),0); // отправка
				//}
				
			}
			
				
		}
	}
	pthread_join(tid, NULL);
	//msgctl(msg_id, IPC_RMID, &qstatus ); управление очередью

	return 0;
}
