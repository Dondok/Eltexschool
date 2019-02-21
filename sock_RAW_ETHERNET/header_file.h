/*
 * Заголовочный файл с общей информацией клиента и сервера.
 */
	
	//08:00:27:ce:95:0e
	#define MY_DEST_MAC0 0x08
	#define MY_DEST_MAC1 0x00
	#define MY_DEST_MAC2 0x27
	#define MY_DEST_MAC3 0x51
	#define MY_DEST_MAC4 0x68
	#define MY_DEST_MAC5 0xa9
	
	//mac 50:b7:c3:ee:10:f8 for wlp3s0
	#define MY_SOURCE_MAC0	0x50
	#define MY_SOURCE_MAC1	0xb7
	#define MY_SOURCE_MAC2	0xc3
	#define MY_SOURCE_MAC3	0xee
	#define MY_SOURCE_MAC4	0x10
	#define MY_SOURCE_MAC5	0xf8

	#define port_server 54321
	#define port_client 12345

	#define h_eth_len 14
	#define h_ip_len 20
	#define h_udp_len 8
	
	
	#define buf_size 1024
	#define packet_size (h_eth_len + h_ip_len + h_udp_len + buf_size)
	#define h_all_p_len (h_eth_len + h_ip_len + h_udp_len)
	

	// Размер ip заголовка в 4-байтных словах

	#define ETH_ALEN	6		/* Октеты в одном адресе ethernet	 */
	#define ETH_P_ALL	0x0003		/* перехватывает каждый пакет */
	#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
	
	
	
	struct sockaddr_ll 
	{
		unsigned short int sll_family;    /* Всегда AF_PACKET */
		unsigned short int sll_protocol;  /* Прот. физического уровня*/
		int	sll_ifindex;   /* Hомер интерфейса */
		unsigned short int sll_hatype;    /* Тип заголовка */
		unsigned char   sll_pkttype;   /* Тип пакета */
		unsigned char   sll_halen;     /* Длина адреса */ 
		unsigned char   sll_addr[8];   /* Адрес физического уровня */
	};
