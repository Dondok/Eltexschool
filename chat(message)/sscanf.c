
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pthread.h>
int main(void)
{
//char str[80];
//int i;
struct msqid_ds qstatus;
	key_t key;
	
	int msg_id;
	
	key = ftok("key_msg",'A'); // создание ключа для очереди
	msg_id = msgget(key, IPC_CREAT| 0666 ); // присоединение к очереди
	msgctl(msg_id, IPC_RMID, &qstatus ); //управление очередью
//sscanf("hello 1 2 3 4 5", "%s%d", str, &i);
//sprintf (str, "%s %d ", "one", 2);
//printf("%s ", str);
return 0;
}
