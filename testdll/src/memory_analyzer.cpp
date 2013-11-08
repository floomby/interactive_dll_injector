#include "../include/memory_analyzer.h"

memory_analyzer::memory_analyzer()
{
    PROCESS_MEMORY_COUNTERS mem_info;

    //this is not quite the right thing
    GetProcessMemoryInfo(GetCurrentProcess(), &mem_info, mem_info.cb);
}

memory_analyzer::~memory_analyzer()
{
    //dtor
}
