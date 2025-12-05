#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <networkconfig.h>
 
#define MAXINTERFACES 16    /* 最大接口数 */
 

int getIPAddrInfo(IP_INFO_T *ip_info)
{
	int fd;         /* 套接字 */
	int if_len;     /* 接口数量 */
	struct ifreq buf[MAXINTERFACES];    /* ifreq结构数组 */
	struct ifconf ifc;                  /* ifconf结构 */
	
    /* 建立IPv4的UDP套接字fd */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
    {   
        perror("socket(AF_INET, SOCK_DGRAM, 0)");
        return -1; 
    }   
 
    /* 初始化ifconf结构 */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
 
    /* 获得接口列表 */
    if (ioctl(fd, SIOCGIFCONF, (char *) &ifc) == -1) 
    {
        perror("SIOCGIFCONF ioctl");
        return -1;
    }

    if_len = ifc.ifc_len / sizeof(struct ifreq); /* 接口数量 */
    //printf("\nport numbers:%d\n", if_len);
	while (if_len--  > 0) /* 遍历每个接口 */
	  {
	  	  if(strncmp(buf[if_len].ifr_name,"lo",2)==0) continue;
		  //printf("port：%s\n", buf[if_len].ifr_name); /* 接口名称 */
		  
		  /* 获得接口标志 */
		  if (!(ioctl(fd, SIOCGIFFLAGS, (char *) &buf[if_len])))
		  {
			  /* 接口状态 */
			  #if USE_NETWORK_DEBUG
			  if (buf[if_len].ifr_flags & IFF_UP)
			  {
				  printf("port status: UP\n");
			  }
			  else
			  {
				  printf("port status: DOWN\n");
			  }
			  #endif
		  }
		  else
		  {
			  char str[256];
			  sprintf(str, "SIOCGIFFLAGS ioctl %s", buf[if_len].ifr_name);
			  perror(str);
		  }
	
		/* IP地址 */
		 if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[if_len])))
		 {
		 	 #if USE_NETWORK_DEBUG
			 printf("IP address:%s\n",
					 (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			 #endif
			 ip_info->address = ((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr.s_addr;
			 
		 }
		 else
		 {
			 char str[256];
			 sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
			 perror(str);
		 }
		
		/* 子网掩码 */
		  if (!(ioctl(fd, SIOCGIFNETMASK, (char *) &buf[if_len])))
		  {
		  	 #if USE_NETWORK_DEBUG
			  printf("subnet mask:%s\n",
					  (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			#endif
				ip_info->netmask = ((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr.s_addr;
		  }
		  else
		  {
			  char str[256];
			  sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
			  perror(str);
		  }
		
		/* 广播地址 */
		  if (!(ioctl(fd, SIOCGIFBRDADDR, (char *) &buf[if_len])))
		  {
		  	 #if USE_NETWORK_DEBUG
			  printf("broadcast address:%s\n",
					  (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
			#endif
		  }
		  else
		  {
			  char str[256];
			  sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
			  perror(str);
		  }

	  /*MAC地址 */
		if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[if_len])))
		{
			 #if USE_NETWORK_DEBUG
			printf("MAC地址:%02x:%02x:%02x:%02x:%02x:%02x\n\n",
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[0],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[1],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[2],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[3],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[4],
					(unsigned char) buf[if_len].ifr_hwaddr.sa_data[5]);
			#endif
			
			memcpy(ip_info->mac,buf[if_len].ifr_hwaddr.sa_data,6);
			
		}
		else
		{
			char str[256];
			sprintf(str, "SIOCGIFHWADDR ioctl %s", buf[if_len].ifr_name);
			perror(str);
		}
	}//–while end
 //关闭socket
    close(fd);
    return 0;
}

#if(0)
int SetGateWay(const char *szGateWay)  
{  
    int ret = 0;    
    char cmd[128];  
    QString DefGW = GetGateWay();  
  
    const char *strGW = DefGW.latin1();   
    
    strcpy(cmd, "route del default gw ");  
    strcat(cmd, strGW);  
    ret = system(cmd);  
    if(ret < 0)  
    {   
        perror("route error");  
        return -1;  
    }   
    strcpy(cmd, "route add default gw ");  
    strcat(cmd, szGateWay);  
    
	 ret = system(cmd);  
		if(ret < 0)
		{
			perror("route error");
			return -1;
		}
	
		return ret;
	}
#endif
#if 0
int  getGateWay(void)
{
    FILE *fp;
    char buf[512];
    char cmd[128];
    char gateway[30];
    char *tmp;
	uint_8 resault = 0;
	struct in_addr addr;
	#if USE_FOR_ARM
		strcpy(cmd, "route");
	#else
   		strcpy(cmd, "ip route");
	#endif
    fp = popen(cmd, "r");
    if(NULL == fp)
    {
        perror("route error");
        return -1;  
    }else{
	    while(fgets(buf, sizeof(buf), fp) != NULL)
	    {
	        tmp =buf;
	        while(*tmp && isspace(*tmp))
	            ++ tmp;
	        if(strncmp(tmp, "default", strlen("default")) == 0)
	        {
	          	resault = 1;
	          	break;
	        }
	    }
	    pclose(fp);
	}
	if(1 != resault) return -1;// not support cmd "ip route"
	
	#if USE_FOR_ARM
	 	sscanf(buf, "%*s%s", gateway);
	#else
    	sscanf(buf, "%*s%*s%s", gateway);
	#endif
	inet_aton(gateway,&addr);
	semaphore_p(volSemPtr->write_cfg_semId);
	params_linux_flashPtr->fnet_params.gateway= addr.s_addr;
	semaphore_v(volSemPtr->write_cfg_semId);
	#if USE_NETWORK_DEBUG
    printf("default gateway:%s\n", gateway);
	#endif
	return 0;
    //return QString(gateway);  
}
#endif
#if 0
int getDNSServer(void)
{
	#define LINEBUFFSIZE 256
	FILE* fd;
	uint_8 *tmp;
	uint_8 resault = 0;
	uint_8 lineBuff[LINEBUFFSIZE];
	uint_8 DNSServer[32];
	struct in_addr addr;
	
	memset(lineBuff,0x00,LINEBUFFSIZE);
	if((fd = fopen(DNS_SERVER, "r"))==NULL)
	{
		return -1;
	}
	else
	{
		//(n = fread(buf, 1, sizeof(buf), f)) fread(fa,4,5,fp); 其意义是从fp所指的文件中，每次读4个字节(一个实数)送入实数组fa中，连续读5次，即读5个实数到fa中。
		//while(fread(lineBuff,1,sizeof(lineBuff), fd) > 0)
		while(fgets(lineBuff,LINEBUFFSIZE,fd) != NULL)
	    {
	    #if USE_NETWORK_DEBUG
    		printf("DNS:%s\n", lineBuff);
		#endif
	       tmp =lineBuff;
		   	if (*tmp=='#') continue;
        	if (*tmp=='\0') continue;
        	if (*tmp=='\n') continue;
	        while(*tmp && isspace(*tmp))
	            ++ tmp;
	        if(strncmp(tmp, "nameserver", strlen("nameserver")) == 0)
	        {
	          	resault = 1;
	          	break;
	        }
	    }
		fclose(fd);
	}
	if(1 != resault) return -1;
	sscanf(lineBuff, "%*s%s", DNSServer);
	inet_aton(DNSServer,&addr);
	if(semaphore_p(volSemPtr->write_cfg_semId))
	{
		params_linux_flashPtr->dns_params.address1= addr.s_addr;
		semaphore_v(volSemPtr->write_cfg_semId);
		#if USE_NETWORK_DEBUG
    	printf("DNS:%s\n", DNSServer);
		#endif
	}

}
#endif

/********************FW_printenv*************************
bootdelay=2
baudrate=115200
bootcmd=run bootcmd_nand
bootargs_base=setenv bootargs console=ttyAM0,115200n8
bootcmd_nand=run bootargs_base bootargs_nand; nand read 0x42000000 200000 300000; bootm
serverip=192.168.102.1
ethaddr=60:19:29:ff:ff:ff
bootargs_nand=setenv bootargs ${bootargs} ubi.mtd=2 root=ubi0:rootfs rootfstype=ubifs rw gpmi ip=${ipaddr}:${serverip}:${gatewayip}:${netmask} fec_mac=${ethaddr}
ipaddr=192.168.107.234
netmask=255.255.255.0
gatewayip=192.168.107.254
****************************************************/
#if 0
unsigned char isDHCP(void)
{
	FILE *fp;
	char buf[512];
	char cmd[128];
	char *tmp;
	uint_8 resault = 0;

	strcpy(cmd, "fw_printenv");
	if((fp = popen(cmd, "r"))==NULL)
	{
		perror("fw_printenv");
		return;  
	}
	while(fgets(buf, sizeof(buf), fp) != NULL)
	{
		tmp =buf;
		if(strstr(tmp, "ip=dhcp") != NULL)
		{
			resault = 1;
			break;
		}
	}
	pclose(fp);
	if(1 != resault) return '0';// 0 means static ip
	return '1';// means dhcp

}
#endif
