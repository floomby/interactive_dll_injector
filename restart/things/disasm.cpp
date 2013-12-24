#include "disasm.h"

disasm::disasm(uint8_t *data, size_t length)
{
    name << "tmp" << rand() << ".bin";

    tmp.open(this->name.str().c_str(), std::ofstream::binary | std::ofstream::out);

    for(int i = 0; i < length; ++i){
        tmp.put(data[i]);
    }

    tmp.close();
}

//I will add the feature to disasemble from
//stdin to nasm to replace the file stuff
void disasm::write()
{
    char cwd_buf[1024];
    std::string cmd_str(_getcwd(cwd_buf, sizeof(cwd_buf)));
    cmd_str += "\\ndisasm.exe";
    std::string arg_str;
    arg_str += cmd_str.c_str();
    arg_str += " -b 32 ";
    arg_str += this->name.str();

    //SECURITY_ATTRIBUTES tmp_atribs;

    //tmp_atribs.nLength = sizeof(SECURITY_ATTRIBUTES);
    //tmp_atribs.lpSecurityDescriptor = NULL;
    //tmp_atribs.bInheritHandle = TRUE;

    PROCESS_INFORMATION proc_info;
    STARTUPINFO start_info = {sizeof(STARTUPINFO)};

    if(!CreateProcess((char *)cmd_str.c_str(),
                      (char *)arg_str.c_str(),
                      NULL,
                      NULL,
                      FALSE,
                      0,
                      NULL,
                      NULL,
                      &start_info,
                      &proc_info)
                      ){
        std::cerr << "Error running disasembler " << GetLastError() << std::endl;
        return;
    }

    WaitForSingleObject(proc_info.hProcess, INFINITE);

    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
}

disasm::~disasm()
{
    // TODO delete the file
}