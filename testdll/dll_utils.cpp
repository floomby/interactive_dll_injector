#include "dll_utils.h"

static HINSTANCE moduleName;

void util::init(HINSTANCE mod_name)
{
    moduleName = mod_name;
}

//unloading is significantly harder than I thought
//therefore this function does nothing

void util::unload()
{
    FreeLibrary(moduleName);
    MessageBoxA(NULL, "Attempting to Unload Injected DLL", "target", MB_OK);
}
