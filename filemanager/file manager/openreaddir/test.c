#include <stdio.h>
#include <dirent.h>
//список функций
/*
 DIR* opendir(const char *name)
 DIR* fdopendir(int fd)
 оба возвращает указатель на структуру с описанием каталога
 
 */
int main()
{
	DIR *dir;
	struct dirent *ent;
	char directory[255]="./";
	dir = opendir(directory);
	
	while ((ent = readdir(dir)) != NULL)
	{
		printf("%s\n", ent->d_name);
	}
    closedir(dir);
    return 0;
}
