#include <stdio.h>

int main(){
int	n=15;
int mass[n];
int mass1[n];	
for(int i=-7; i<n;i++){
	mass[i]=i;
	printf("%i  ",mass[i]);
	
	}	
	printf("\n  ");	
for(int i=-7;i<n;i++){
if(mass[i]<0){
	mass1[i]=mass[i];
	printf("%i  ",mass1[i]);
	}	
}
return 0;	
}
