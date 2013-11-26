#ifndef DLLTEMPLATE_H_INCLUDED
#define DLLTMEPLATE_H_INCLUDED

#include <windows.h>

#include "piper.h"

extern "C" __declspec(dllexport) void module_init(void (*[])(), piper *);

#endif // DLLTMEPLATE_H_INCLUDED
