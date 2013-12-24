#include "framework.h"

#include <iostream>

using std::string;
using std::to_string;

using std::cerr;
using std::endl;

namespace framework
{

void erl_sender(fw *inst)
{
    while(!inst->closed){
        while(!inst->sending.empty()){
            send(inst->sock, inst->sending.front().c_str(), inst->sending.front().length(), 0);
            inst->sending.pop();
        }
        Sleep(10);
    }
}

void erl_receiver(fw *inst)
{
    char buf[4096];
    int bytes;

    while(!inst->closed){
        ZeroMemory(buf, sizeof(buf));
        bytes = recv(inst->sock, buf, sizeof(buf) - 1, 0);
        
        if(bytes > 0){
            inst->received.push(string(buf));
        }else if(bytes == 0){
            inst->closed = true;
        }else{
            cerr << "conection error " << WSAGetLastError() << endl;
            inst->closed = true;
        }
    }
}

fw::fw(const char *server, const char *port)
{
    this->sock = INVALID_SOCKET;
    this->closed = false;
    this->erl_connect(server, port);
    
    // create threads to send and recieve
    this->sender = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&erl_sender, (LPVOID)this, 0, NULL);
    this->receiver = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&erl_receiver, (LPVOID)this, 0, NULL);
}

fw::~fw()
{
    while((!this->received.empty()) || (!this->sending.empty())){
        Sleep(10);
    }

    // delete sending and reciving threads
    TerminateThread(this->sender, 0);
    TerminateThread(this->receiver, 0);
    
    this->erl_cleanup();
}

// fixme: this function should fail if no one is listening (not the erl_receiver function)

void fw::erl_connect(const char *server, const char *port)
{
    int ret;
    WSADATA wsaData;

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
        WSACleanup();
        cerr << "error: getaddrinfo " << ret << endl;
        exit(EXIT_FAILURE);
    }

    for(ptr = result; ptr; ptr = ptr->ai_next){
        this->sock = socket(ptr->ai_family,
                            ptr->ai_socktype,
                            ptr->ai_protocol);

        if(this->sock == INVALID_SOCKET){
        WSACleanup();
        cerr << "error: socket " << WSAGetLastError() << endl;
        exit(EXIT_FAILURE);
        }

        if(connect(this->sock, ptr->ai_addr, (int)ptr->ai_addrlen)){
            closesocket(this->sock);
            this->sock = INVALID_SOCKET;
            continue;
        }
    }

    freeaddrinfo(result);
}

void fw::erl_cleanup() throw()
{
    closesocket(this->sock);
    
    WSACleanup();
}

} // end namespace famework