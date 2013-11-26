#include <cstdio>
#include <ctime>

#include <windows.h>

extern "C"
void call_me()
{
    puts("you win");
    fflush(stdout);
}

int main()
{
    time_t a_time, b_time;

    puts("Just try to debug me");
    fflush(stdout);

    for(;;){
        time(&a_time);
        if(IsDebuggerPresent())
            MessageBoxA(NULL, "Debbuger Detected", "target", MB_OK);
        Sleep(500);
        time(&b_time);
        if(difftime(a_time, b_time) > 1)
            MessageBoxA(NULL, "Debbuger Detected", "target", MB_OK);
    }
    return 0;
}
