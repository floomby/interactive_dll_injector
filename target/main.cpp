#include <cstdio>

#include <windows.h>

extern "C"
void call_me()
{
    puts("you win");
}

int main()
{
    MessageBoxA(NULL, "this is a useless program", "target", MB_OK);
    for(;;){
        if(IsDebuggerPresent())
            MessageBoxA(NULL, "Debbuger Detected", "target", MB_OK);
        Sleep(500);
    }
    return 0;
}
