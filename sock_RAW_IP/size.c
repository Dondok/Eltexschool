#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "header_file.h"


int main()
{
	struct IP_Header IP;
	struct ip_header ip2;
	printf("%i\n",sizeof(IP));
	printf("%i\n",sizeof(ip2));
	return 0;
}
