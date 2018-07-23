/*ДЗ с 23.07.18
 * задание: создать потоки, при котором каждый поток должен выводить
 * свой номер
 */
#include <stdio.h>
#include <pthread.h>
void *func(void *ptr)
{
	int *ptr1 = ptr;
	printf("%d\n",*ptr1);
	//завершаем поток
	pthread_exit(0);
	return 0;
}

int main()
{
	int i=5;
	int mass[i];
	//создаем идентификатор потока
	pthread_t tid;
	int ptr;
	for (i = 1; i <= 5; i++){
		mass[i] = i;
		//создаем поток по идентификатору tid и функции потока func
		//и передаем потоку указатель на данные mass[i]
		ptr=pthread_create(&tid, NULL, func,&mass[i]);
	}
	//ждем завершения потока
	pthread_join(tid, NULL);
	
	return 0;
}
