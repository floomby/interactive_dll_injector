#include <iostream>

#include "../framework/framework.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3){
        cerr << "usage: " << argv[0] << " <server> <port>" << endl;
        exit(EXIT_FAILURE);
    }

    // framework::init(argv[0]);
    framework::erl_connect(argv[1], argv[2]);
    
    framework::erl_send("blah");
    
    framework::process();
    
    //TODO should not be done here
    WSACleanup();

    return 0;
}