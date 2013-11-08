#ifndef PIPER_H
#define PIPER_H

#include <cstring>
#include <cstdlib>

#include <windows.h>

#include "../dll_utils.h"

class piper
{
    public:
        piper(const char *pipe_name);
        virtual ~piper();
        void write(const char *str);
        void raw_write(void *bytes, size_t len);
        char *read();
    protected:
    private:
        HANDLE hPipe;
        DWORD cnt;
        char buf[4096];
        const char *done = "";
};

#endif // PIPER_H
