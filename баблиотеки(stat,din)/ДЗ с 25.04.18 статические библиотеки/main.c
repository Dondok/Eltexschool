/*	дата создания 25.06.18
 * 	задача: написать программу "калькулятор" с использованием динамической
 *	библиотеки.
 */
#include "calc.h"
#include <stdio.h>


int main()
{
	
	int a, b;
	int input;
	//ввод а и b
	printf("введите: число 'a'");
	scanf( "%i", &a );
	printf("введите: число 'b'");
	scanf( "%i", &b );
	
	printf("введите: \n1-сумма\n 2-разность\n 3-деление \n 4-умножение\n5-выход из программы\n");
    
    scanf( "%d", &input );
    switch ( input )
    {
        case 1:
			printf("a+b= %i\n", sum(a,b));
			break;
            
        case 2:
            printf("a-b= %i\n",dif(a,b));
			break;
            
        case 3:
            printf("a/b= %i\n",div(a,b));
			break;
            
        case 4:
			printf("a*b= %i\n", mult(a,b));
            break;
            
        case 5:
            break; 
            
        default:
            printf( "Неправильный ввод.\n" );
    }

	return 0;	
}
