#include "piper.h"

static const char *greeting = "stager loaded";

piper::piper(const char *pipe_name)
{
    this->hPipe = CreateNamedPipe(pipe_name,
                                  PIPE_ACCESS_DUPLEX,
                                  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                                  2, 0, 0, 0, NULL);

    if(this->hPipe == INVALID_HANDLE_VALUE){
        MessageBoxA(NULL, "Pipe Creation Error", "Target", MB_OK);
        //TODO: unload the dll, exit thread
        ExitProcess(MY_PIPE_ERROR);
    }

    if(!ConnectNamedPipe(this->hPipe, NULL)){
        MessageBoxA(NULL, "Pipe Connection Error", "Target", MB_OK);
        //TODO: unload dll, exit thread
        CloseHandle(this->hPipe);
        ExitProcess(MY_PIPE_ERROR);
    }

    WriteFile(this->hPipe, greeting, strlen(greeting), &cnt, NULL);
}

piper::~piper()
{
    FlushFileBuffers(this->hPipe);
    DisconnectNamedPipe(this->hPipe);
    CloseHandle(this->hPipe);
}

//TODO return number of bytes writen
void piper::write(const char *str)
{
    WriteFile(this->hPipe, str, strlen(str), &(this->cnt), NULL);
}

//TODO need a real protocol here this one sucks
void piper::raw_write(void *bytes, size_t len)
{
    if(len > (sizeof(this->buf) - sizeof(size_t))){
        this->write("raw_write: size to large");
        return;
    }

    *(size_t *)buf = len;
    memcpy(buf + sizeof(size_t), bytes, len);

    WriteFile(this->hPipe, buf, sizeof(buf), &(this->cnt), NULL);
}


char *piper::read()
{
    //potential to segfault in this functions
    memset(&(this->buf), 0, sizeof(this->buf));

    if(!ReadFile(this->hPipe, this->buf, sizeof(this->buf), &(this->cnt), NULL) || this->cnt == 0){
        if(GetLastError() == ERROR_BROKEN_PIPE){
            MessageBoxA(NULL, "Handler Disconect", "target", MB_OK);
            //Unload dll so we can re-inject if we want
            return (char *)this->done;
        }else{
            MessageBoxA(NULL, "Unknown Pipe Error", "target", MB_OK);
            return (char *)this->done;
        }
    }

    return buf;
}
