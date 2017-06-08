#include "changeformat.h"
#include "skbuff.h"
#include "eth.h"
#include "arp.h"
#include "ip.h"
#include "udp.h"
#include "utils.h"
#include "2410lib.h"

char TftpLoadEnd;
char TftpPutBegin;
char TftpPutMark;
char* ptr;
/*RTC*/
extern char*  rtc_test(void);

int net_handle(void)
{
	struct sk_buff *skb;
	struct ethhdr *eth_hdr;												
	skb = alloc_skb(ETH_FRAME_LEN);

	if (eth_rcv(skb) != -1) 
	{

		eth_hdr = (struct ethhdr *)(skb->data);					
		skb_pull(skb, ETH_HLEN);
		if (ntohs(eth_hdr->h_proto) == ETH_P_ARP)
		{
			arp_rcv_packet(skb);
		}

		else if(ntohs(eth_hdr->h_proto) == ETH_P_IP)						
		{
		 	ip_rcv_packet(skb);
		}
	 	
	}

	free_skb(skb);

	return 0;
}



#define	LOCAL_IP_ADDR	((192UL<<24)|(168<<16)|(2<<8)|111)

extern unsigned long download_len;
extern unsigned long download_addr;

int NetLoadFile(UINT32T addr, UINT32T give_ip, UINT32T a3, UINT32T a4)
{
	struct sk_buff *skb = alloc_skb(ETH_FRAME_LEN);
	unsigned char eth_addr[ETH_ALEN];	
	unsigned char *s;
	int i;
	char *p;
	give_ip = LOCAL_IP_ADDR;
	s = (unsigned char *)&give_ip;
	
	uart_printf("Mini TFTP Server 1.0 (IP : %d.%d.%d.%d PORT: %d)\n", s[3], s[2], s[1], s[0], TFTP);		
	uart_printf("Type tftp -i %d.%d.%d.%d put filename at the host PC\n", s[3], s[2], s[1], s[0]);

	eth_init();		
	eth_get_addr(eth_addr);		
	ip_init(give_ip);
	udp_init();
		
	arp_add_entry(eth_addr, give_ip);	
	while(1){
			uart_printf("udp send\n");
			net_handle();
	
			udp_skb_reserve(skb);		
			
			ptr = rtc_test();
			memcpy(skb->data, ptr, 7);

			udp_send(skb, 3232235627, UDP, 5678);

			delay(1000);
	}
	return 0;
}



