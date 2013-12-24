#include <windows.h>
#include <memory.h>

#include "../params.h"

// TODO shared memory

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

// this is realatively safe since we expect that user.dll is loaded
extern "C" __declspec(dllexport) char *test(void *param)
{
    MessageBoxA(NULL, "test", "test", MB_OK);
}

