#include "../include/prompter.h"

//TODO we need to detect when the server closes and quit

using namespace std;

prompter::prompter(piper *apipe)
{
    this->the_pipe = apipe;
}

void prompter::prompt()
{
    //i am not going to use a lookuptable here
    //however in the DLL i am, due to more options
    char cmd;
    string arg;

    for(;;){
        cout << "~>" << flush;
        cin >> cmd;
        if(cmd == 'h'){
            this->help();
            continue;
        }
        if(cmd == 'q'){
            return;
        }

        cout << "~~~>" << flush;
        cin >> arg;

        string tmp("");
        tmp += cmd;
        tmp += arg;
        this->the_pipe->write(tmp.c_str());

        if(cmd == 'd'){
            //Sleep(500);
            disasm adis(this->the_pipe->raw_read());
            adis.write();

            //for(uint8_t c : this->the_pipe->raw_read()){
            //    cout << hex << setw(2) << setfill('0') << (int)c << " ";
            //}
            //cout << endl;
            continue;
        }

        cout << this->the_pipe->read() << endl;
    }
}

void prompter::help()
{
    cout << "Help:" << endl;
    cout << "\th - help" << endl;
    cout << "\te - echo" << endl;
    cout << "\tk - kill" << endl;
    cout << "\tq - quit" << endl;
    cout << "\td - disasm" << endl;
    cout << "\tc - call" << endl;
}
