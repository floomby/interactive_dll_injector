#ifndef PIPER_H
#define PIPER_H

#include <windows.h>
#include <cstring>
#include <iostream>
#include <string>
#include <cstdint>

class piper
{
    public:
        piper(const char *pipe_name);
        virtual ~piper();
        std::string read();
        std::basic_string<uint8_t> raw_read();
        void write(const char *str);
    protected:
    private:
        HANDLE hPipe;
        char buf[4096];
        DWORD cnt;
};

#endif // PIPER_H
