/*
 * Заголовочный файл с общей информацией клиента и сервера.
 */

#define port_server 1999
#define port_client 2222

#define UDP_head_len 8
#define IP_head_len 20

#define buf_size 1024
#define packet_size (IP_head_len + UDP_head_len + buf_size)

// Размер ip заголовка в 4-байтных словах
#define IP_head_for_packer (IP_head_len / 4)

#define TOTAL_len (UDP_head_len + IP_head_len +buf_size)





struct UdpHeader
{
	u_short source_port;
	u_short dest_port;
	u_short length;
	u_short checksum;
};

struct IP_Header
{
	u_int8_t ver_head_len; /* версия, длина заголовка*/
	u_int8_t ip_tos; /* тип сервиса */
	u_int16_t total_len; /* общая длина */
	u_int16_t ip_id; /* идентификатор пакета*/
	u_int16_t ip_off; /* смещение*/
	u_int8_t ip_ttl; /* время жизни */
	u_int8_t ip_type_prot; /* тип протокола след. уровня*/
	u_int16_t ip_check_sum; /* контрольная сумма заголовка */
	u_int32_t ip_src;	/* IP адрес источника */
    u_int32_t ip_dest;	/* IP адрес назначение */
	
};

struct packet
{
	struct IP_Header h_ip;
	struct UdpHeader h_udp;
	char payload[buf_size];
};
