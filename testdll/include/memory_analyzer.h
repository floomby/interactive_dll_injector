#ifndef MEMORY_ANALYZER_H
#define MEMORY_ANALYZER_H

#include <windows.h>
#include <Psapi.h>

//haveing some stupid problems since windows 8

#ifndef PSAPI_WORKING_SET_EX_BLOCK

typedef union _PSAPI_WORKING_SET_EX_BLOCK {
    ULONG_PTR Flags;
    struct {
        ULONG_PTR Valid  :1;
        ULONG_PTR ShareCount  :3;
        ULONG_PTR Win32Protection  :11;
        ULONG_PTR Shared  :1;
        ULONG_PTR Node  :6;
        ULONG_PTR Locked  :1;
        ULONG_PTR LargePage  :1;
        ULONG_PTR Reserved  :7;
        ULONG_PTR Bad  :1;
#if defined(_WIN64)
        ULONG_PTR ReservedUlong  :32;
#endif
    };
    struct {
        ULONG_PTR Valid  :1;
        ULONG_PTR Reserved0  :14;
        ULONG_PTR Shared  :1;
        ULONG_PTR Reserved1  :15;
        ULONG_PTR Bad  :1;
#if defined(_WIN64)
        ULONG_PTR ReservedUlong  :32;
#endif
    } Invalid;
} PSAPI_WORKING_SET_EX_BLOCK, *PPSAPI_WORKING_SET_EX_BLOCK;

#endif // PSAPI_WORKING_SET_EX_BLOCK

#ifndef PSAPI_WORKING_SET_EX_INFORMATION

typedef struct _PSAPI_WORKING_SET_EX_INFORMATION {
    PVOID                       VirtualAddress;
    PSAPI_WORKING_SET_EX_BLOCK  VirtualAttributes;
} PSAPI_WORKING_SET_EX_INFORMATION, *PPSAPI_WORKING_SET_EX_INFORMATION;

#endif // PSAPI_WORKING_SET_EX_INFORMATION

class memory_analyzer
{
    public:
        memory_analyzer(void *addr);
        virtual ~memory_analyzer();
    protected:
    private:
};

#endif // MEMORY_ANALYZER_H
