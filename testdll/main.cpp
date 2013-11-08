#include <cstdio>
#include <cstring>
#include <windows.h>

#include "include/piper.h"
#include "dll_utils.h"
#include "cmds.h"

#define PIPE_NAME "\\\\.\\pipe\\apipe"

piper *mypipe = nullptr;

//calling needs to be convention aware
//the way that comes to mind right now
//is to use assemmbly exerps to call functions

void call(void *addr)
{
    ((void (*)())(addr))();
}

//void dump(void *addr, size_t len)
//{
//    mypipe->raw_write((uint8_t *)addr, len);
//}

void thread_main();

extern "C"
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason){
        case DLL_PROCESS_ATTACH:
            //MessageBoxA(NULL, "Injection Sucessful", "It Worked", MB_OK);
            mypipe = new piper(PIPE_NAME);
            thread_main();
            //dump((void *)0x00401331, 24);
            //call((void *)0x00402AD0);
            break;
        case DLL_PROCESS_DETACH:
            //not in kernel32.dll so potentialy
            //bad to call message box(but it should work)
            MessageBoxA(NULL, "Dll Unloaded", "target", MB_OK);
            //delete mypipe;
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}

#define p(x) (x-0x61)

void cmd_default(char *arg){ mypipe->write("unrecognized cmd"); }

static void* cmd_table[] = {
(void *)&cmd_default,   // "a"
(void *)&cmd_default,   // "b"
(void *)&cmd_default,   // "c"
(void *)&cmd_default,   // "d"
(void *)&cmd_default,   // "e"
(void *)&cmd_default,   // "f"
(void *)&cmd_default,   // "g"
(void *)&cmd_default,   // "h"
(void *)&cmd_default,   // "i"
(void *)&cmd_default,   // "j"
(void *)&cmd_default,   // "k"
(void *)&cmd_default,   // "l"
(void *)&cmd_default,   // "m"
(void *)&cmd_default,   // "n"
(void *)&cmd_default,   // "o"
(void *)&cmd_default,   // "p"
(void *)&cmd_default,   // "q"
(void *)&cmd_default,   // "r"
(void *)&cmd_default,   // "s"
(void *)&cmd_default,   // "t"
(void *)&cmd_default,   // "u"
(void *)&cmd_default,   // "v"
(void *)&cmd_default,   // "w"
(void *)&cmd_default,   // "x"
(void *)&cmd_default,   // "y"
(void *)&cmd_default    // "z"
};

//for registering functions as commands
void register_func(char cmd, void(*func)(char *));
void call_func(char cmd, char *arg);

//called after pipe is constructed
void thread_main()
{
    char *str = nullptr; //, *cmd_str = nullptr, *arg_str = nullptr;

    cmd::init(mypipe);
    register_func('e', &cmd::echo);
    register_func('k', &cmd::kill);
    register_func('d', &cmd::dump);
    register_func('c', &cmd::call);

    for(;;){
        str = mypipe->read();
        call_func(*str, str + 1);

        delete[] str;
    }
}

void call_func(char cmd, char *arg)
{
    if(cmd < 'a' || 'z' < cmd){
        mypipe->write("invalid command");
        return;
    }

    ((void (*)(char *))cmd_table[p(cmd)])(arg);
}

void register_func(char cmd, void (*func)(char *))
{
    if(cmd < 'a' || 'z' < cmd){
        mypipe->write("invalid command registration");
        return;
    }

    cmd_table[p(cmd)] = (void *)func;
}
