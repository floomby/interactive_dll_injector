#include "cmds.h"

#define p(x) (x-0x61)

namespace cmd
{

static piper *mypipe = nullptr;

void cmd_default(char *arg){ mypipe->write("unrecognized cmd"); }

static void (*cmd_table[26])(char *) = {&cmd_default};

void init(piper *pipe)
{
    mypipe = pipe;

    register_func('e', &echo);
    register_func('k', &kill);
    register_func('d', &disasm);
    register_func('c', &call);
}

void call_func(char cmd, char *arg)
{
    if(cmd < 'a' || 'z' < cmd){
        mypipe->write("invalid command");
        return;
    }

    (cmd_table[p(cmd)])(arg);
}

void register_func(char cmd, void (*func)(char *))
{
    if(cmd < 'a' || 'z' < cmd){
        mypipe->write("invalid command registration");
        return;
    }

    cmd_table[p(cmd)] = func;
}

void echo(char *arg)
{
    mypipe->write(arg);
}

void kill(char *arg)
{
    delete mypipe;
    ExitProcess((unsigned)atoi(arg));
}

void disasm(char *arg)
{
    //32 bit only
    void *addr;

    sscanf(arg, "%x", (unsigned *)&addr);

    analyze ana((uint8_t *)addr);

    //mypipe->write(ana.message);
    //since bad messaging desing
    //Sleep(500);
    mypipe->raw_write(addr, ana.len());
}

//need to spawn a new thread to do the call

void call(char *arg)
{
    //32 bit only
    void *addr;

    sscanf(arg, "%x", (unsigned *)&addr);
    ((void (*)())(addr))();

    /*
    char buf[200] = {0};
    void *addr;

    sscanf(arg, "%x", (unsigned *)&addr);

    DWORD thread_id;
    HANDLE call_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)addr, NULL, 0, &thread_id);

    MessageBoxA(NULL, "blah", "target", MB_OK);

    if(!call_thread){
        mypipe->write("error creating thread");
        return;
    }

    Sleep(500);

    sprintf(buf, "%lx", thread_id);
    mypipe->write(buf);

    SuspendThread(call_thread);

    */
    /*
    uint32_t ret;

    ((void (*)())addr)();

    MessageBoxA(NULL, "call returned", "target", MB_OK);

    __asm__(
        ".intel_syntax;"
        "mov %0, eax;"
        ".att_syntax;"
        : "=r"(ret) : : "eax"
    );

    sprintf(buf, "Assuming cdcel/stdcel/fastcel calling convention function returned %u", ret);
    mypipe->write(buf);
    */
}

} //end namespace cmd
