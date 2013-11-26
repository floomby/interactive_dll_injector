#include <windows.h>
#include <Psapi.h>

#include "../common/piper.h"

static piper *my_pipe = nullptr;

void register_funcs(void *funcs);

extern "C"
__declspec(dllexport) void module_init(void *funcs, piper *pipe)
{
    my_pipe = pipe;

    register_funcs(funcs);
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
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

void echo(const char *arg)
{
    my_pipe->write(arg);
}

void register_funcs(void *funcs)
{
    ((void **)funcs)['e'-0x61] = (void *)&echo;
    my_pipe->write("functions registered");
}
