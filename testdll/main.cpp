#include <cstdio>
#include <cstring>
#include <windows.h>

#include "include/piper.h"
#include "dll_utils.h"
#include "cmds.h"

#define PIPE_NAME "\\\\.\\pipe\\apipe"

//It will be important to avoid alocating
//on the heap at all cost after injection
//that beig said there are still ways to
//use the heap to a limited degree

//additionally if Psapi.dll is not loaded
//and we are attempiting to be stealthy
//we can load just the functions we need

//memory snapshoting can be done in several
//ways, the one I am implementing now uses
//Psapi functions called from the injected
//dll, But an external program can snapshot
//memory of a program with attaching a
//debugger (using the same functions and
//OpenProcess), however this is less stealthy

piper *mypipe = nullptr;

//calling needs to be convention aware
//the way that comes to mind right now
//is to use assemmbly exerps to call functions

void thread_main();

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason){
        case DLL_PROCESS_ATTACH:
            //MessageBoxA(NULL, "Injection Sucessful", "It Worked", MB_OK);
            mypipe = new piper(PIPE_NAME);
            thread_main();
            //dump((void *)0x00401331, 24);
            //call((void *)0x00402AD0);
            break;
        case DLL_PROCESS_DETACH:
            //not in kernel32.dll so potentialy
            //bad to call message box(but it should work)
            MessageBoxA(NULL, "Dll Unloaded", "target", MB_OK);
            //delete mypipe;
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

//called after pipe is constructed
void thread_main()
{
    char *str = nullptr; //, *cmd_str = nullptr, *arg_str = nullptr;

    cmd::init(mypipe);

    for(;;){
        str = mypipe->read();
        cmd::call_func(*str, str + 1);

        delete[] str;
    }
}
