NMCFind_UdpClientTool.cpp  --- 源码
NMCFind_UdpClientTool.exe  --- 编译出来的工具

工具使用方法：
NMCFind_UdpClientTool.exe IPSegment

举例：
NMCFind_UdpClientTool.exe 10.94.66
---工具会从10.94.66.1扫描到10.94.66.255并在DOS界面打印结果。


打印结果说明：
IP  recvfrom()返回值|对应的失败原因值错误码
返回值为接收到的探测响应的字节数，<=0表示失败。
对应的失败原因值错误码：https://learn.microsoft.com/zh-cn/windows/win32/winsock/windows-sockets-error-codes-2