#include "cmds.h"

static piper *mypipe = nullptr;

void cmd::init(piper *pipe)
{
    mypipe = pipe;
}

void cmd::echo(char *arg)
{
    mypipe->write(arg);
}

void cmd::kill(char *arg)
{
    delete mypipe;
    ExitProcess((unsigned)atoi(arg));
}

void cmd::dump(char *arg)
{
    //32 bit only
    void *addr;
    size_t len;

    sscanf(arg, "%x,%u", (unsigned *)&addr, (unsigned *)&len);

    analyze ana((uint8_t *)addr);

    mypipe->raw_write(addr, ana.len());
}

//need to spawn a new thread to do the call

void cmd::call(char *arg)
{
    void *addr;

    sscanf(arg, "%x", (unsigned *)&addr);

    ((void (*)())addr)();
}
