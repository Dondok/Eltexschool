//	дата создания:	19.06.18
#include <stdio.h>
#include <stddef.h>
struct test {
	
			char a;
			int b;
		
			};
struct testtest {
	
			char a;
			int b;
			
			    }__attribute__((packed));			

int main()
{
	char str[10] = {'A',0,0,0,0,'B',0,0,0,0};

	
   for(int i=0; i<10; i++)
    {
    printf("str[i] = %c\n", str[i]);
	}
  
    printf("sizeof(struct test)      = %i\n", (int)sizeof(struct test));
    printf("sizeof(struct testtest)  = %i\n", (int)sizeof(struct testtest));
   
    
    return 0;
}
