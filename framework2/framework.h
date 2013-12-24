#ifndef FRAMEWORK_H_INCLUDED
#define FRAMEWORK_H_INCLUDED

#undef UNICODE

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <direct.h>

#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>

namespace framework
{

class fw {
public:
    fw(const char *server, const char *port);
    virtual ~fw();

    std::queue<std::string> received;
    std::queue<std::string> sending;
    
    friend void erl_sender(fw *inst);
    friend void erl_receiver(fw *inst);
private:
    SOCKET sock;

    // handles to the threads
    HANDLE sender, receiver;

    bool closed;
    
    void erl_connect(const char *server, const char *port);
    void erl_cleanup() throw();
};

} // end namespace framework



#endif//FRAMEWORK_H_INCLUDED
