#include "cmd.h"

#define p(x) (x-0x61)

namespace cmd
{

static piper *my_pipe = nullptr;

void invalid(const char *arg){ my_pipe->write("unrecognized cmd"); }

static void (*table[26])(const char *) = {&invalid};

void init(piper *pipe)
{
    my_pipe = pipe;

    add('l', &load);
}

void call(char cmd, const char *arg)
{
    if(cmd < 'a' || 'z' < cmd){
        my_pipe->write("invalid command");
        return;
    }

    (table[p(cmd)])(arg);
}

void add(char cmd, void (*func)(const char *))
{
    if(cmd < 'a' || 'z' < cmd){
        my_pipe->write("invalid command registration");
        return;
    }

    table[p(cmd)] = func;
}

typedef void (*module_init)(void *);

void load(const char *arg)
{
    HINSTANCE hDllInstance = LoadLibrary(arg);

    if(hDllInstance == NULL){
        my_pipe->write("could not load dll");
        return;
    }

    module_init mod_init = (module_init)GetProcAddress(hDllInstance, "module_init");

    if(!mod_init){
        my_pipe->write("could not locate module_init");
        return;
    }

    mod_init(table, my_pipe);
}

} //end namespace cmd
