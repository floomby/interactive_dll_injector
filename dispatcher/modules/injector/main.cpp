#include <cstdio>
#include <cstring>

#include <windows.h>

#include "../module.h"

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

extern "C" __declspec(dllexport) char *inject(unsigned ProcessID, const char *dllName)
{
    HANDLE Proc;
    LPVOID RemoteString, LoadLibAddy;

    if(!ProcessID){ 
        char *ret = (char *)malloc(strlen(ERR_MSG));
        strcpy(ret, ERR_MSG);
        return ret;
    }

    Proc = OpenProcess(CREATE_THREAD_ACCESS, false, ProcessID);

    if(!Proc){
        char *ret = (char *)malloc(strlen(ERR_MSG));
        strcpy(ret, ERR_MSG);
        return ret;
    }

    //fixme: it would be nice if we could error check the injection
    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(dllName), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL);
    CreateRemoteThread(Proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, 0, NULL);

    CloseHandle(Proc);

    char *ret = (char *)malloc(strlen(OK_MSG));
    strcpy(ret, OK_MSG);
    return ret;
}

extern "C" BOOL APIENTRY __declspec(dllexport) DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason){
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}