/*	дата создания 27.06.18
 * 	задача: написать программу "калькулятор" с использованием динамической
 * библиотеки(явное связывание).
 */
#include <dlfcn.h>
#include <stdio.h>



/* В dlfcn.h определены следующие функции:
void* dlopen("PATH_AND_NAME",FLAG) - загружает в память динамическую
* библиотеку с полным именем PATH_AND_NAME и возвращает ее описатель (HANDLE)
*  (NULL в случае неудачи).
*  FLAG - флаги, описанные в "man dlopen";
void* dlsym(HANDLE,"NAME") - возвращает указатель на функцию/переменную,
*  импортируемую из библиотеки;
int dlclose(HANDLE) - выгружает библиотеку из памяти;
const char *dlerror() - получить сообщение о последней возникшей ошибке
*  (NULL - если ошибок не произошло с момента последнего вызова dlerror).
 */
 

/*Данный оператор сообщает компилятору о необходимости распознавания 
 * func как другого имени для int.
 */
typedef int (*func)(int, int);


int main()
{	
	int a, b;
	int input;
	func sum,dif,div,mult;
	char *error;
	printf("введите занчение a=");
	scanf( "%d", &a );
	printf("\nвведите занчение b=");
	scanf( "%d", &b );
			//открываем библиотеки, RTLD_LAZY-"ленивое связывание(читать man dlopen)"
			void* handle_sum = dlopen("/usr/lib/libsum.so",RTLD_LAZY);
			void* handle_dif = dlopen("/usr/lib/libdif.so",RTLD_LAZY);
			void* handle_div = dlopen("/usr/lib/libdiv.so",RTLD_LAZY);
			void* handle_mult = dlopen("/usr/lib/libmult.so",RTLD_LAZY);
			
			
	printf("введите: \n1-сумма\n 2-разность\n 3-деление \n 4-умножение\n5-выход из программы\n");
    scanf( "%d", &input );
    //в операторе swich мы отределяем какую операцию(сложение, разность и т.д.) будем использовать
    switch ( input )
    {		
		
		
       case 1:
			//в dlsym передаем дескриптор и название функции и переопределяем как тип (func)
			sum=(func)dlsym(handle_sum,"sum");
				printf("a+b= %i\n",sum(a,b));
				break; 
            
            
        case 2:
			dif=(func)dlsym(handle_dif,"dif");
			
			//функция dlerror возвращает удобочитаемую строку с описанием последней ошибки
				error = dlerror();
				if (error != NULL)
					{
							printf( "!!! %s\n", error );
					}
				printf("a-b= %i\n",dif(a,b));
				break;    
				
				
        case 3:
			div=(func)dlsym(handle_div,"div");
				printf("a/b= %i\n",div(a,b));
				break;
            
            
        case 4:
			mult=(func)dlsym(handle_mult,"mult");
				printf("a*b= %i\n",mult(a,b));
				break;
            
            
        case 5:
            break; 
        default:
            printf( "Неправильный ввод.\n" );
    }
    //закрываем открытые библиотеки
	dlclose(handle_sum);
	dlclose(handle_dif);
	dlclose(handle_div);
	dlclose(handle_mult);
	return 0;	
}
