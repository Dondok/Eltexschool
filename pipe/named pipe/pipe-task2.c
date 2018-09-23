#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
#define max 100

int main()
{
    int fd;
 
    // пусть к файлу FIFO
    char * myfifo = "/tmp/myfifo";
    // Создание именованного файла (FIFO)
    // mkfifo(путь, права доступа)
	mkfifo(myfifo, 0666);
    char buf1[max], buf2[max];
    while (1)
    {
        // открываем канал только для записи
        fd = open(myfifo, O_WRONLY);
 
        
        //заполняем буфер
        fgets(buf2, max, stdin);
 
        
        // записываем в канал
        write(fd, buf2, strlen(buf2)+1);
        close(fd);
 
        // открваем канал только для чтения
        fd = open(myfifo, O_RDONLY);
 
        // читаем с канала
        read(fd, buf1, sizeof(buf1));
 
        // выводим сообщение
        printf("User2: %s\n", buf1);
        close(fd);
    }
    
    return 0;
}
