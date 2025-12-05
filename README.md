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