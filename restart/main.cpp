#include <iostream>
#include <iomanip>

#include <cstdlib>

#include <windows.h>

#include "target.h"
#include "things/disasm.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2){
        cerr << "usage: " << argv[0] <<  " <pid>" << endl;
        exit(EXIT_FAILURE);
    }
    
    //load the library
    char cwd_buf[1024];
    string dllName(_getcwd(cwd_buf, sizeof(cwd_buf)));
    dllName += DLL_NAME;
    
    if(!LoadLibraryA(dllName.c_str())){
        cerr << "failed to load library " << GetLastError() << endl;
        exit(EXIT_FAILURE);
    }
    
    target *test = nullptr;
    uint8_t *buf = nullptr;
    
    try
    {
        test = new target((DWORD)atoi(argv[1]));
        Sleep(1000);
        
        test->call("test", NULL);
        buf = test->read((void *)0x4017F0, 48);
    }
    catch (const char *err)
    {
        cerr << err << endl;
        exit(EXIT_FAILURE);
    }
    
    disasm blah(buf, 48);
    blah.write();
    
    delete test;
    
	return 0;
}
