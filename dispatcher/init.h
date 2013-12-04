#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include <iostream>

#undef UNICODE

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

SOCKET init(const char *server, const char *port);

#endif//INIT_H_INCLUDED