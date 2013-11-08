#ifndef DLL_UTILS_H_INCLUDED
#define DLL_UTILS_H_INCLUDED

#include <windows.h>

namespace util
{
void init(HINSTANCE mod_name);
void unload();
}

#endif // DLL_UTILS_H_INCLUDED
