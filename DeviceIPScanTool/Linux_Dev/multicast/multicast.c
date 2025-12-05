#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h> 
#include <time.h> 

#include <networkconfig.h>

#define  multicast_ip_addr "233.0.0.6"
//setting
#define		SETTING_BUFF_SIZE  128
#define     IPPORT_GROUP_RECV  9090
#define     IPPORT_GROUP_SEND  9091	
#define grouprxbuff_size_hex 128
#define IPBYTES(a)            ((a>>24)&0xFF),((a>>16)&0xFF),((a>> 8)&0xFF),(a&0xFF)
#define IPBYTES_BIG(a)			(a&0xFF) ,((a>> 8)&0xFF),((a>>16)&0xFF),((a>>24)&0xFF)

typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
//typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
//typedef unsigned long long uint64_t;



int main(int argc, char* argv[])
{
   struct sockaddr_in		 addr,group_addr;
   int			 sock, error;
   int			 inlen;
   int			 addrlen;

   struct  ip_mreq	group;

   uint8_t group_rxbuff[grouprxbuff_size_hex];


   sock = socket(PF_INET, SOCK_DGRAM, 0);
   if (sock < 0)
   {
		perror("socket");  
		return -1;
   }
   usleep(3000);
  //group
		group_addr.sin_family=AF_INET;
		group_addr.sin_addr.s_addr=inet_addr(multicast_ip_addr);//htonl(INADDR_ANY);	
		group_addr.sin_port=htons(IPPORT_GROUP_SEND);
		
   //INADDR_ANY 
   memset(&addr,0,sizeof(addr));  
   addr.sin_family=AF_INET;  
   addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
   addr.sin_port=htons(IPPORT_GROUP_RECV);
   
   addrlen = sizeof(addr);
   /* Bind to UDP port */
   error = bind(sock, (struct sockaddr*)&addr, sizeof(addr));
   if (error < 0) 
   {
   		perror("bind");  
		return -1;
   }
   /*
	group.imr_multiaddr.s_addr =  multicast_ip_addr;
	group.imr_interface.s_addr =  IPCFG_default_ip_address;
	error = setsockopt(sock, SOL_IGMP,RTCS_SO_IGMP_ADD_MEMBERSHIP, &group,sizeof(group));
	*/
   int fneedresetgroupipaddr=1;

   while(1) 
   {
	
		//加入组播
	if(fneedresetgroupipaddr==1)
	{	
		fneedresetgroupipaddr=0;
		//error = setsockopt(sock, SOL_IGMP,RTCS_SO_IGMP_DROP_MEMBERSHIP, &group,sizeof(group));

		usleep(300);
		group.imr_multiaddr.s_addr =  inet_addr(multicast_ip_addr);
		group.imr_interface.s_addr =  htonl(INADDR_ANY);//inet_addr("192.168.107.234");
		//printf("****%x\n%x\n%x\n",inet_addr("127.0.0.0"),inet_addr("192.168.107.234"),IPCFG_default_ip_address);
		
		/*
		different socket vision，IP_ADD_MEMBERSHIP's value is different：
		in socket ver1.0中：
		＃define IP_ADD_MEMBERSHIP 5
		
		in socket ver2.0中：
		＃define IP_ADD_MEMBERSHIP 12

		so use IP_ADD_MEMBERSHIP may be error.
		*/
	//printf("---------IP_ADD_MEMBERSHIP=%d--------",IP_ADD_MEMBERSHIP);
		if (setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&group,sizeof(group)) < 0)   
		{
			//perror("setsockopt");
			fneedresetgroupipaddr=1;
			continue;
		}
		printf("multicast is open\n");
	}
	memset(group_rxbuff,0x00,sizeof(group_rxbuff));
	
	inlen = recvfrom(sock, group_rxbuff, grouprxbuff_size_hex-1, 0, (struct sockaddr*)&addr, &addrlen);
	if(inlen<1)//error data
	{
		continue;
	}
	printf("recv:%s\n",group_rxbuff);
	
	IP_INFO_T ip_info;
	char *pDst=group_rxbuff;
	if(getIPAddrInfo(&ip_info)!=0){
		printf("Get ip infomation error..\n");
		continue;
	}
	
	sprintf(group_rxbuff,"get_sn:0000000;\n");
	
	char hname[64];
	memset(hname, 0, sizeof(hname));
        if (gethostname(hname, sizeof(hname))) {
            memcpy(hname, "NMCGW_NEW_liulin", strlen("NMCGW_NEW_liulin"));
        }
	char model[128];
	memset(model, 0, sizeof(model));
	snprintf(model, sizeof(model) , "get_model_name:%s", hname);
	strcat(group_rxbuff, model);
	strcat(group_rxbuff,";\n");
	
	strcat(group_rxbuff,"get_ip:");
	pDst = group_rxbuff+strlen(group_rxbuff);
	sprintf(pDst,"%d.%d.%d.%d",(int)(ip_info.address&0xff) ,(int)(ip_info.address>>8 &0xff),(int)(ip_info.address>>16 &0xff),(int)(ip_info.address>>24 &0xff));
	strcat(group_rxbuff,";\n");
	
	strcat(group_rxbuff,"get_mac:");
	pDst = group_rxbuff+strlen(group_rxbuff);
	sprintf(pDst,"%02x-%02x-%02x-%02x-%02x-%02x",ip_info.mac[0],ip_info.mac[1],ip_info.mac[2],ip_info.mac[3],ip_info.mac[4],ip_info.mac[5]);
	strcat(group_rxbuff,";\n");	
	
	/*strcat(group_rxbuff,"get_iot_status:---");
	strcat(group_rxbuff,";\n");	
	strcat(group_rxbuff,"get_iot_reason:---");
	strcat(group_rxbuff,";\n");		
	strcat(group_rxbuff,"get_wifi_ip:---");
	strcat(group_rxbuff,";\n");		
	strcat(group_rxbuff,"get_wifi_mac:---");
	strcat(group_rxbuff,";\n");	
	strcat(group_rxbuff,"get_wifi_status:---");
	strcat(group_rxbuff,";\n");		
	strcat(group_rxbuff,"get_wifi_reason:---");
	strcat(group_rxbuff,";\n");			
	strcat(group_rxbuff,"get_wifi_rssi:---");
	strcat(group_rxbuff,";\n");		
	strcat(group_rxbuff,"get_heap_used:---");
	strcat(group_rxbuff,";\n");	
	strcat(group_rxbuff,"get_FW_Version_765:---");
	strcat(group_rxbuff,";\n");	*/
	printf("send:%s",group_rxbuff);
	sendto(sock,group_rxbuff,strlen(group_rxbuff),0,(struct sockaddr*)&addr,addrlen);
	
  }
  
  return 0;
}




