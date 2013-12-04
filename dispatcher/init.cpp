#include "init.h"

using namespace std;

SOCKET init(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;

    SOCKET Sock = INVALID_SOCKET;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))){
        cerr << "error: WSAStartup " << ret << endl;
        exit(EXIT_FAILURE);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if((ret = getaddrinfo(server, port, &hints, &result))){
        cerr << "error: getaddrinfo " << ret << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        Sock = socket(ptr->ai_family,
                      ptr->ai_socktype,
                      ptr->ai_protocol);

        if(Sock == INVALID_SOCKET){
            cerr << "error: socket " << WSAGetLastError() << endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        if(connect(Sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(Sock);
            Sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);

    return Sock;
}