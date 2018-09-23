#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define max 100

int main()
{
    int fd1;
 
    // FIFO file path
    char * myfifo = "/tmp/myfifo";
 
    // создаем именованный канал
    // mkfifo(<путь>,<права доступа>)
    mkfifo(myfifo, 0666);
 
    char str1[max], str2[max];
    while (1)
    {
        // открываем канал и читаем
        fd1 = open(myfifo,O_RDONLY);
        read(fd1, str1, max);
 
        // выводим сообщение
        printf("User1: %s\n", str1);
        close(fd1);
 
        fd1 = open(myfifo,O_WRONLY);
        fgets(str2, max, stdin);
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
    }
    return 0;
}
