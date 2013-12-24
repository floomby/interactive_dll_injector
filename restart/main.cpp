#include <iostream>
#include <iomanip>

#include <cstdlib>

#include "../framework2/framework.h"

#include "target.h"
#include "things/disasm.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3){
        cerr << "usage: " << argv[0] <<  " <server> <port>" << endl;
        exit(EXIT_FAILURE);
    }
    
    framework::fw the_fw(argv[1], argv[2]);
    
    //load the library
    char cwd_buf[1024];
    string dllName(_getcwd(cwd_buf, sizeof(cwd_buf)));
    dllName += DLL_NAME;
    
    if(!LoadLibraryA(dllName.c_str())){
        cerr << "failed to load library " << GetLastError() << endl;
        exit(EXIT_FAILURE);
    }
    
    target *test = nullptr;
    
    try
    {
        test = new target((DWORD)atoi(argv[]));
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
