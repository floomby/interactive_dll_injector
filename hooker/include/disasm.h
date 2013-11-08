#ifndef DISASM_H
#define DISASM_H

#include <cstdio>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include <windows.h>

class disasm
{
    public:
        disasm(std::basic_string<uint8_t> data);
        void write();
        virtual ~disasm();
    protected:
    private:
        std::ofstream tmp;
        std::stringstream name;
};

#endif // DISASM_H
