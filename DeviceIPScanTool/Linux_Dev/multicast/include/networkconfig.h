#ifndef _NETWORKCONFIG_H_
#define _NETWORKCONFIG_H_

#define DNS_SERVER "/etc/resolv.conf"
#define USE_NETWORK_DEBUG (0)


typedef struct 
{
    unsigned long address;
    unsigned long netmask; 
    unsigned long gateway; 
    unsigned char mac[6]; 
    unsigned char DHCPactive;
    unsigned char mcu_flag;
}IP_INFO_T;

int getIPAddrInfo(IP_INFO_T *ip_info);
//int getGateWay(void);
//int getDNSServer(void);
//unsigned char isDHCP(void);


#endif

