/*
 * Заголовочный файл с общей информацией клиента и сервера.
 */

#define port_server 1999
#define port_client 2222

#define UDP_head_len 8
#define IP_head_len 20

#define buf_size 1024
#define packet_size (IP_head_len + UDP_head_len + buf_size)

struct sockaddr_in server,client;

struct UdpHeader
{
	u_short source_port;
	u_short dest_port;
	u_short length;
	u_short checksum;
};

