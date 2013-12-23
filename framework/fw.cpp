/**
 * This is the file for the code in
 * the framework that needs socket
 * access
 *
 */

#include "framework.h"

#include <iostream>

using std::cout;
using std::endl;

using std::unordered_map;
using std::to_string;
using std::string;
using std::vector;

namespace framework
{

fw::fw(const char *server, const char *port, const char *name)
{
    this->name = string(name);
    
    this->wraper = new wrap();
    
    this->sock = INVALID_SOCKET;
    this->erl_connect(server, port);
}

fw::~fw()
{
    delete this->wraper;

    this->erl_cleanup();
}

void fw::erl_cleanup()
{
    closesocket(this->sock);
    
    WSACleanup();
}


void fw::erl_connect(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    if((ret = WSAStartup(MAKEWORD(2,2), &wsaData))){
        string err("error: WSAStartup ");
        err += to_string(ret);
        throw err;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    
    if((ret = getaddrinfo(server, port, &hints, &result))){
        WSACleanup();
        string err("error: getaddrinfo ");
        err += to_string(ret);
        throw err;
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        this->sock = socket(ptr->ai_family,
                            ptr->ai_socktype,
                            ptr->ai_protocol);

        if(this->sock == INVALID_SOCKET){
        WSACleanup();
        string err("error: socket ");
        err += to_string(WSAGetLastError());
        throw err;
        }

        if(connect(this->sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);

    //send the framework node name and then get the responce
    erl_send(this->name.c_str());
    this->msgs.push(erl_recv());
}

void fw::process()
{
    if(this->sock == INVALID_SOCKET){
        throw "not connected";
    }
    
    string input;
    
    for(;;){
        input = erl_recv();

        cout << input.c_str() << endl;
        
        char *ret = this->wraper->call(this->erl_recv());
        this->erl_send(ret);
        free(ret);
    }

    this->erl_cleanup();
}

void fw::erl_send(const char *str)
{
    send(this->sock, str, strlen(str), 0);
}

string fw::erl_recv()
{
    char buf[4096] = {'\0'};
    int bytes;
    
    bytes = recv(this->sock, buf, sizeof(buf) - 1, 0);
    
    if(bytes > 0){
        ;
    }else if(bytes == 0){
        throw "connection closed";
    }else{
        throw "error: recv";
    }
    
    return string(buf);
}

} //end namespace framework
