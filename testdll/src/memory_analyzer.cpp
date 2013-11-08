#include "../include/memory_analyzer.h"

//the two key functions here are
//GetMappedFileName and QueryWorkingSet

memory_analyzer::memory_analyzer(void *addr)
{
    //not sure exactly how to do this
    PSAPI_WORKING_SET_EX_INFORMATION mem_info;

    mem_info.VirtualAddress = addr;

    QueryWorkingSet(GetCurrentProcess(), &mem_info, sizeof(PSAPI_WORKING_SET_INFORMATION));
}

memory_analyzer::~memory_analyzer()
{
    //dtor
}
