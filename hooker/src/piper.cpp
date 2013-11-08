#include "../include/piper.h"

using std::cerr;
using std::endl;

piper::piper(const char *pipe_name)
{
    this->hPipe = CreateFile(pipe_name,
                             GENERIC_READ | GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);

    if (hPipe == INVALID_HANDLE_VALUE){
		cerr << "Invalid Pipe Handle:" << GetLastError() << endl;
		exit(EXIT_FAILURE);
	}

    //dont realy know if I need this but I am being explicit
	this->cnt = 0;
}

piper::~piper()
{
    CloseHandle(this->hPipe);
}

//figure out the error handling
std::string piper::read()
{
    char buf[4096];

    if(!ReadFile(this->hPipe, buf, sizeof(buf), &(this->cnt), NULL) || this->cnt == 0){
        cerr << "Server Quit/Other Pipe Error";
        return std::string("");
    }

    return std::string(buf);
}

//TODO return number of bytes writen
void piper::write(const char *str)
{
    WriteFile(this->hPipe, str, strlen(str), &(this->cnt), NULL);
}

//this function need some work
//needs to catch errors from other thing
std::basic_string<uint8_t> piper::raw_read()
{
    uint8_t buf[4096];
    size_t len;

    if(!ReadFile(this->hPipe, buf, sizeof(buf), &(this->cnt), NULL) || this->cnt == 0){
        if(GetLastError() == ERROR_BROKEN_PIPE){
            cerr << "Server Quit" << endl;
        }else{
            cerr << "Handler: Unknown Pipe Error(raw read)" << endl;
        }
        return std::basic_string<uint8_t>('\0');
    }

    len = *(size_t *)buf;

    if(len > (4096 - sizeof(size_t))){
        cerr << "Invalid Length in raw_read" << endl;
    }

    return std::basic_string<uint8_t>(buf + sizeof(size_t), len);
}
