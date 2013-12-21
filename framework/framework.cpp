/**
 * This is the file for the code in
 * the framework that needs socket
 * access
 *
 */

#include "framework.h"

#ifndef QUIET
#include <iomanip>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
#endif//QUIET

#include <unordered_map>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::unordered_map;

#include "wrap.h"

namespace framework
{

static SOCKET the_socket = INVALID_SOCKET;

bool erl_connect(char *server, char *port)
{
    int ret;
    WSADATA wsaData;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))){
#ifndef QUIET
        cerr << "error: WSAStartup " << ret << endl;
#endif//QUIET
        return true;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if((ret = getaddrinfo(server, port, &hints, &result))){
#ifndef QUIET
        cerr << "error: getaddrinfo " << ret << endl;
#endif//QUIET
        WSACleanup();
        return true;
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        the_socket = socket(ptr->ai_family,
                            ptr->ai_socktype,
                            ptr->ai_protocol);

        if(the_socket == INVALID_SOCKET){
#ifndef QUIET
            cerr << "error: socket " << WSAGetLastError() << endl;
#endif//QUIET
            WSACleanup();
            return true;
        }

        if(connect(the_socket, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(the_socket);
            the_socket = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);

    return false;
}

bool process()
{
    if(the_socket == INVALID_SOCKET){
#ifndef QUIET
        cerr << "not connected" << endl;
#endif//QUIET
        return true;
    }

    char buf[4096];
    int bytes;

    unordered_map<string, string> loaded;

    for(;;){
        memset(buf, 0, sizeof(buf));

        bytes = recv(the_socket, buf, sizeof(buf), 0);
        if(bytes > 0){
            call(buf, &loaded);
        }else if(bytes == 0){
#ifndef QUIET
            cout << "connection closed" << endl;
#endif//QUIET
            break;
        }else{
#ifndef QUIET
            cerr << "error: recv " << WSAGetLastError() << endl;
#endif//QUIET
            break;
        }
    }

    closesocket(the_socket);
    
    return false;
}

bool erl_send(const char *str)
{
    return (bool)send(the_socket, str, strlen(str), 0);
}

} //end namespace framework
