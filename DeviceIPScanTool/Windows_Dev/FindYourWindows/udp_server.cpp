/**
********************************************************************************
**
**  Copyright (C) 2025 Eaton
**
**  This software is confidential and licensed under Eaton Proprietary License
**  (EPL or EULA).
**  This software is not authorized to be used, duplicated or disclosed to
**  anyone without the prior written permission of Eaton.
**  Limitations, restrictions and exclusions of the Eaton applicable standard
**  terms and conditions, such as its EPL and EULA, apply.
**
********************************************************************************
*/

/*****
A UDP server which hold 9090 UDP port to response server info to client.
*****/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>

#include "udp_server.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library


static char g_respoinse[BUFLEN] = { "Hello, it is me!\0" };


bool udp_server()
{
    //Initialise winsock
    LOG("\nInitialising Winsock...\n");
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        LOG("Failed. Error Code : %d\n", WSAGetLastError());
        return false;
    }
    LOG("Initialised.\n");

    //Create a socket
    SOCKET s;
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        LOG("Could not create socket : %d\n", WSAGetLastError());
    }
    LOG("Socket created.\n");

    //Prepare the sockaddr_in structure
    struct sockaddr_in server, si_other;
    int slen = sizeof(si_other);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        LOG("Bind failed with error code : %d\n", WSAGetLastError());
        return false;
    }
    LOG("Bind done!\n");

    //keep listening for data
    int recv_len = 0;
    char buf[BUFLEN];
    while (1)
    {
        LOG("Waiting for data...\n");
        fflush(stdout);

        //clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            LOG("recvfrom() failed with error code : %d\n", WSAGetLastError());
            continue;
        }

        //print details of the client/peer and the data received
        LOG("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        LOG("Data: %s\n", buf);

        //now reply the client with the same data
        if (sendto(s, g_respoinse, strlen(g_respoinse), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            LOG("sendto() failed with error code : %d\n", WSAGetLastError());
            continue;
        }
    }

    closesocket(s);
    WSACleanup();

    return true;
}