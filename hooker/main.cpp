#include <windows.h>
#include <stdio.h>
//#include <tlhelp32.h>
//#include <shlwapi.h>

//TODO c++ style IO

#include <iostream>
#include <iomanip>

using namespace std;

#include "include/piper.h"
#include "include/prompter.h"

#define PIPE_NAME "\\\\.\\pipe\\apipe"
#define DLL_NAME "C:\\Users\\Josh\\Desktop\\hooker\\testdll.dll"

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

BOOL InjectDLL(DWORD ProcessID, const char *dllName);

#define IsWindowsNT() (GetVersion()<0x80000000)

int main(int argc, char *argv[])
{
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << "<PID>" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(IsWindowsNT()){
        if(!(InjectDLL(atoi(argv[1]), DLL_NAME)))
            fprintf(stderr, "Injection failed");
    }else{
        fprintf(stderr, "Not windows NT\n");
    }

    Sleep(500);
    piper mypipe(PIPE_NAME);

    //do prompt now

    std::cout << mypipe.read() << std::endl;

    /*for(uint8_t c : mypipe.raw_read()){
        std::cout << std::hex << std::setw(2) << (int)c << " ";
    }*/

    prompter myprompt(&mypipe);
    myprompt.prompt();

    mypipe.~piper();

    return 0;
}

BOOL InjectDLL(DWORD ProcessID, const char *dllName)
{
    HANDLE Proc;
    LPVOID RemoteString, LoadLibAddy;

    if(!ProcessID)
        return false;

    Proc = OpenProcess(CREATE_THREAD_ACCESS, false, ProcessID);

    if(!Proc){
        fprintf(stderr, "OpenProcess() failed: %lu\n", GetLastError());
        return false;
    }

    LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(DLL_NAME), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL);
    CreateRemoteThread(Proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, 0, NULL);

    CloseHandle(Proc);

    return true;
}
