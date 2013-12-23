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

class wrap
{
public:
    wrap();
    virtual ~wrap();
    char *call(std::string input);
    
private:
    char *call_i(std::vector<std::string> *args, std::vector<std::string> *types, FARPROC fp);
};

class fw
{
public:
    fw(const char *server, const char *port, const char *name);
    virtual ~fw();
    
    void process();
   
private:
    std::string name;
    SOCKET sock;
    
    void erl_connect(const char *server, const char *port);
    void erl_cleanup();
    
    void erl_send(const char *str);
    std::string erl_recv();
    
    std::queue<std::string> msgs;
    wrap *wraper;
};

namespace dll_utils
{

void load(std::string module);
FARPROC address(std::string module, std::string function);

} //end namespace dll_utils

} //end namespace framework


#endif//FRAMWORK_H_INCLUDED
