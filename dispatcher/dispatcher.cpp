#include <iostream>

#include "../framework/framework.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3){
        cerr << "usage: " << argv[0] << " <server> <port>" << endl;
        exit(EXIT_FAILURE);
    }
    
    try
    {
    try
    {
    framework::fw test_fw(argv[1], argv[2], "dispatcher");
    
    test_fw.process();
    }
    catch (char *err)
    {
    // TODO fix problems with the exception handleing
    cerr << err << endl;
    exit(EXIT_FAILURE);
    }
    }
    catch (string err)
    {
    cout << err.c_str() << endl;
    }
    return 0;
}