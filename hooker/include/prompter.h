#ifndef PROMPTER_H
#define PROMPTER_H

#include <iostream>
#include <iomanip>
#include <cstdio>

#include "piper.h"
#include "disasm.h"

class prompter
{
    public:
        prompter(piper *apipe);
        void prompt();
    protected:
    private:
        piper *the_pipe;
        void help();
};

#endif // PROMPTER_H
