#ifndef DISASM_H
#define DISASM_H

#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include <windows.h>

#include "direct.h"

class disasm
{
    public:
        disasm(uint8_t *data, size_t length);
        void write();
        virtual ~disasm();
    protected:
    private:
        std::ofstream tmp;
        std::stringstream name;
};

#endif // DISASM_H
