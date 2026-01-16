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

#pragma once


#define PORT 9090	//The port on which to listen for incoming data
#define BUFLEN 512	//Max length of buffer


#define DEBUG 0

#if DEBUG
#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) ((void)0)
#endif



bool udp_server();