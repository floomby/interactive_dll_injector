#include "target.h"


using std::make_pair;
using std::vector;
using std::string;

#define CREATE_THREAD_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ)

target::target(DWORD ProcessID)
{
    this->pid = ProcessID;
    
    char cwd_buf[1024];
    this->dllName = new string(_getcwd(cwd_buf, sizeof(cwd_buf)));
    (*dllName) += DLL_NAME;
    
    this->Proc = OpenProcess(CREATE_THREAD_ACCESS, false, this->pid);
    
    if(!this->Proc){
        throw "invalid process";
    }

    LPVOID LoadLibAdd = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

    this->RemoteString = (LPVOID)VirtualAllocEx(this->Proc, NULL, this->dllName->length() + 1, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(this->Proc, this->RemoteString, dllName->c_str(), this->dllName->length() + 1, NULL);
    CreateRemoteThread(this->Proc, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibAdd, this->RemoteString, 0, NULL);
    
    this->dllHandle = GetModuleHandle(this->dllName->c_str());
    this->remote_free_list.push_back(make_pair(this->RemoteString, this->dllName->length() + 1));
}

target::~target()
{   
    // TODO unload the dll
    
    // TODO look into the problems that freeing the memory createss
    //free the memory that we used to load the dll
    //VirtualFreeEx(Proc, this->RemoteString, dllName->length() + 1, MEM_DECOMMIT);
    
    CloseHandle(Proc);
    
    delete this->dllName;
}

void target::call(const char *name, void *param)
{
    //TODO use VirtualAllocEx or SHARED MEMORY to get the parameter to the target

    //think like: void (*fp)(void *)
    LPTHREAD_START_ROUTINE fp = (LPTHREAD_START_ROUTINE)GetProcAddress(this->dllHandle, (LPCSTR)name);
    
    CreateRemoteThread(this->Proc, NULL, 0, fp, param, 0, NULL);
}


// THIS FUNCTION IS NOT THREAD SAFE
void target::write(void *what, size_t length)
{
    LPVOID space = VirtualAllocEx(this->Proc, NULL, length, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(Proc, space, what, length, NULL);
    
    this->remote_free_list.push_back(make_pair(what, length));
}

// TODO fix the leak
uint8_t *target::read(void *where, size_t length)
{
    uint8_t *buf = new uint8_t[length];
    SIZE_T bytes;
    
    ReadProcessMemory(this->Proc, where, buf, length, &bytes);
 
    return buf;
}

