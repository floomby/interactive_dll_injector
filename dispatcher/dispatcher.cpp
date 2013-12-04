///fixme:there is a really strange bug (do "injector:inject()")
///fixme:memory leak in wrap.cpp

#undef UNICODE

#include <cstdlib>
#include <cstdlib>
#include <cassert>

#include <direct.h>

#include <iostream>
#include <iomanip>

#include <unordered_map>
#include <vector>

using namespace std;

#include "wrap.h"
#include "init.h"

void loop(SOCKET Sock);
bool load(string module, unordered_map<string, string> *dllMap);
void call(char *cmd, SOCKET Sock, unordered_map<string, string> *dllMap);

int main(int argc, char *argv[])
{
    if(argc != 3){
        cerr << "usage: " << argv[0] << " <server> <port>" << endl;
        exit(EXIT_FAILURE);
    }

    loop(init(argv[1], argv[2]));

    WSACleanup();

    return 0;
}

bool load(string module, unordered_map<string, string> *dllMap)
{
    char cwd_buf[1024];
    string dllName(_getcwd(cwd_buf, sizeof(cwd_buf)));
    
    dllName += "\\modules\\" + module + "\\" + module + ".dll";
    
    if(!LoadLibraryA(dllName.c_str())){
        return true;
    }
    
    dllMap->insert(make_pair<string, string>(module.c_str(), dllName.c_str()));
    return false;
}

void loop(SOCKET Sock)
{
    char buf[4096];
    int bytes;

    unordered_map<string, string> loaded;

    for(;;){
        memset(buf, 0, sizeof(buf));

        bytes = recv(Sock, buf, sizeof(buf), 0);
        if(bytes > 0){
            call(buf, Sock, &loaded);
        }else if(bytes == 0){
            cout << "connection closed" << endl;
            break;
        }else{
            cerr << "error: recv " << WSAGetLastError() << endl;
            break;
        }
    }

    closesocket(Sock);
}

void call(char *cmd, SOCKET Sock, unordered_map<string, string> *dllMap)
{
    string module(strtok(cmd, ":"));

    string func(strtok(NULL, "("));

    char *args_raw = strtok(NULL, ")");

    vector<string> args;

    if(args_raw){
        args.push_back(strtok(args_raw, ","));

        char *it = NULL;

        while((it = strtok(NULL, ","))){
            args.push_back(string(it));
        }
    }

    if(!dllMap->count(module)){
        if(!load(module, dllMap)){
            send(Sock, "loading module", strlen("loading module"), 0);
        }else{
            send(Sock, "failed to  load module", strlen("failed to  load module"), 0);
            return;
        }
    }

    void *fp = (void *)GetProcAddress(GetModuleHandle(module.c_str()), func.c_str());

    func.append("TYPE");

    const char *types_raw = *(const char **)GetProcAddress(GetModuleHandle(module.c_str()), func.c_str());    
    
    char *types_raw_cpy = (char *)malloc(strlen(types_raw));
    assert(types_raw_cpy);
    strcpy(types_raw_cpy, types_raw);

    vector<string> types;

    if(strlen(types_raw_cpy) > 1){
        types.push_back(strtok(types_raw_cpy, ","));

        char *it = NULL;

        while((it = strtok(NULL, ","))){
            types.push_back(string(it));
        }
    }

    char *ret = wrap(&args, &types, fp);

    send(Sock, ret, strlen(ret), 0);

    free(types_raw_cpy);
    free(ret);
}