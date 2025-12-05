# DeviceIPScanTool
A tool used to find device IP when you only know its IP segment.

# How to use
1. run multicast tool(only support linux_x86, for linux_arm or other linux, build it from source code) on you linux deivce as a daemon;   
2. when you want to find your device, run NMCFind_UdpClientTool.exe as ./Windows_Host/Tool readme.txt.   

# Directory Structure
./DeviceIPScanTool:    
1. Linux_Dev:  
multicast: source code of multicast tool.  
Tool: multicast tool built from multicast source code.  
2. Windows_Host:  
NMCFind_UdpClientTool: source code of NMCFind_UdpClientTool.exe tool.  
Tool: NMCFind_UdpClientTool.exe tool.  

# NMCFind_UdpClientTool.exe output
```
10.9.8.7  -1|10060

10.9.8.8
recv msg: get_sn:0000000;
get_model_name:My Workstation;
get_ip:10.9.8.8;
get_mac:00-01-02-03-04-05;

10.9.8.9  -1|10060
```