#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>

static struct sockaddr_in address;
static struct socket *sock;
static unsigned short port = 80;
static unsigned char dst_addr[4] = {8,8,8,8};

/*
 * используемый структуры для рассмотрения
 * */
 
/*
struct msghdr {
	void		*msg_name;			ptr to socket address structure 
	int		msg_namelen;			size of socket address structure 
	struct iov_iter	msg_iter;	 	data 
	void		*msg_control;		ancillary data 
	__kernel_size_t	msg_controllen; ancillary data buffer length 
	unsigned int	msg_flags;		flags on received message 
	struct kiocb	*msg_iocb;		ptr to iocb for async requests 
};
 */
/*
struct kvec {
	void *iov_base; and that should *never* hold a userland pointer
	size_t iov_len;
};
*/

static u32 create_address(u8 *ip)
{
    u32 addr = 0;
    int i;
    for (i = 0; i < 4; ++i)
    {
        addr += ip[i];
        if(i == 3) break;
        addr <<= 8;
    }
    return addr;
}

unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void construct_header(struct msghdr * msg, struct sockaddr_in * address){
    msg->msg_name = address;
    msg->msg_namelen = sizeof(struct sockaddr_in);
    msg->msg_control = NULL;
    msg->msg_controllen = 0;
    msg->msg_flags = 0;
}

static int send_icmp(void)
{
	struct icmphdr h_icmp;
    struct msghdr msg;
    struct kvec vec;
    int size_pkt = sizeof(struct icmphdr);
	int count;
	address.sin_addr.s_addr = htonl(create_address(dst_addr));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    construct_header(&msg	,&address);
    
    vec.iov_len = size_pkt;
    vec.iov_base = &h_icmp;
	
	memset((void *)&h_icmp, 0, sizeof(h_icmp));
	
	//заполнение заголовка icmp и отправка.
	for(count = 0; count < 30; count++){
				
		memset(&h_icmp, 0, sizeof(struct icmphdr));
		h_icmp.type = ICMP_ECHO;
		h_icmp.code = 0;
		h_icmp.un.echo.id = 101;
		h_icmp.un.echo.sequence = count;
		h_icmp.checksum = csum((unsigned short *)&h_icmp,
		sizeof(struct icmphdr)/2);
			
		kernel_sendmsg(sock, &msg, &vec, 1, size_pkt);
	}
	
}

static unsigned int hfunc(void *priv, struct sk_buff *skb,
						  const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct icmphdr *icmp;
	char source[16];
	
	if (!skb)
		return NF_ACCEPT;

	iph = ip_hdr(skb);
	if (iph->protocol == IPPROTO_ICMP 
		&& iph->saddr == address.sin_addr.s_addr) {
		
		icmp = icmp_hdr(skb);
		if (icmp->type == 0) {
			
			snprintf(source, 16, "%pI4", &iph->saddr);
			printk("from %s seq = %d  ttl=%d\n", source,
                   ntohs(icmp->un.echo.sequence)/256, iph->ttl);
                    
			return NF_DROP;
		}
	}

	return NF_ACCEPT;
}

static const struct nf_hook_ops nfho = {
	.hook		= hfunc,			/* hook function */
	.pf		    = PF_INET,			/* IPv4 */
	/* hooknum - идентификатор крючка. */
	.hooknum	= NF_INET_PRE_ROUTING,	/* received packets */
	.priority	= NF_IP_PRI_FIRST,		/* max hook priority */
};

static int __init LKM_init(void)
{

	int result;

    result = sock_create(AF_INET, SOCK_RAW, IPPROTO_ICMP, &sock);
    if (result < 0) {
        pr_err("Error %d while creating socket\n", result);
        sock_release(sock);
        return result;
    }
    pr_info("Socket created\n");
	
	send_icmp();

	nf_register_net_hook(&init_net, &nfho);		
}

static void __exit LKM_exit(void)
{
	nf_unregister_net_hook(&init_net, &nfho);
	
	sock_release(sock);
	
	printk("module ping deleted\n");
}

module_init(LKM_init);
module_exit(LKM_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Baldanov D. B.");
