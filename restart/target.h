#ifndef TARGET_H_INCLUDED
#define TARGET_H_INCLUDED

#include <string>
#include <vector>

#include <windows.h>
#include <direct.h>

#define DLL_NAME "\\dll.dll"

class target
{
public:
    target(DWORD ProcessID);
    virtual ~target();
    
    void call(const char *name, void *param);
    void write(void *what, size_t length);
    uint8_t *read(void *where, size_t length);
    
private:
    HANDLE Proc;
    HMODULE dllHandle;
    
    std::vector<std::pair<LPVOID, size_t> > remote_free_list;
    
    LPVOID RemoteString;
    DWORD pid;
    std::string *dllName;
};


#endif//TARGET_H_INCLUDED