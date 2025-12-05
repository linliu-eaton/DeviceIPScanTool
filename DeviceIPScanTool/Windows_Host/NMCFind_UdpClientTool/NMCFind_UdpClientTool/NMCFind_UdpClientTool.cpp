#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
//#include <windows.h>   // 一定不能和winsock2同时存在，会报奇怪的错误
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define DISCOVER_PORT   9090 //6787

int main(int argc, char* argv[])
{
    if (argc <= 1) {
        printf("IP segment error!\n");
        return -1;
    }

    WSADATA wsaData;
    if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
        printf("Init UDP failed!\n");
        return -1;
    }

    SOCKET sclient = socket(AF_INET, SOCK_DGRAM, 0);
    int optval = 1;//这个值一定要设置，否则可能导致sendto()失败
    setsockopt(sclient, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (const char*)&optval, sizeof(int));
    struct timeval TimeOut;
    TimeOut.tv_sec = 5;
    TimeOut.tv_usec = 0;
    setsockopt(sclient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(DISCOVER_PORT);
    sin.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    char serverIP[20];
    char recvData[255];
    for (int i = 1; i <= 255; i++) {
        memset(serverIP, 0, sizeof(serverIP));
        snprintf(serverIP, sizeof(serverIP), "%s.%d", argv[1], i);
        sin.sin_addr.s_addr = inet_addr(serverIP);

        const char* sendData = "{\"scanRequestFrom\":\"sps\"}";
        int len = sizeof(sin);
        printf("%s  ", serverIP);
        sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);
        Sleep(200);

        memset(recvData, 0, sizeof(recvData));
        int ret = recvfrom(sclient, recvData, sizeof(recvData), 0, (sockaddr *)&sin, &len);
        if (ret > 0) {
            printf("\nrecv msg: %s\n", recvData);
        }
        else {
            // 错误码含义：https://learn.microsoft.com/zh-cn/windows/win32/winsock/windows-sockets-error-codes-2
            printf("%d|%d\n", ret, WSAGetLastError());
        }

    }

    closesocket(sclient);
    WSACleanup();
    return 0;
}
