/*
    Simple UDP Server
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include<stdio.h>
#include<winsock2.h>

#include "udp_server.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer
#define PORT 9090	//The port on which to listen for incoming data

int main()
{
    udp_server();


    return 0;
}



