#include "../include/disasm.h"

disasm::disasm(std::basic_string<uint8_t> data)
{
    name << "tmp" << rand() << ".bin";

    tmp.open(this->name.str().c_str(), std::ofstream::binary | std::ofstream::out);

    for(uint8_t c : data){
        tmp.put(c);
    }

    tmp.close();
}

//I will add the feature to disasemble from
//stdin to nasm to replace the file stuff
void disasm::write()
{
    std::string cmd_str("C:\\Users\\Josh\\Desktop\\hooker\\ndisasm.exe");
    std::string arg_str;
    arg_str += cmd_str.c_str();
    arg_str += " -b 32 ";
    arg_str += this->name.str();

    //SECURITY_ATTRIBUTES tmp_atribs;

    //tmp_atribs.nLength = sizeof(SECURITY_ATTRIBUTES);
    //tmp_atribs.lpSecurityDescriptor = NULL;
    //tmp_atribs.bInheritHandle = TRUE;

    PROCESS_INFORMATION proc_info;
    STARTUPINFO start_info;

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
        std::cerr << "Error running disasembler" << std::endl;
        return;
    }

    WaitForSingleObject(proc_info.hProcess, INFINITE);

    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
}

disasm::~disasm()
{
    //delete the file
}
