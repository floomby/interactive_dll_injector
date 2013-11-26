#include "../include/analyze.h"

//for analyzing a function
//something else will identify functions

//the techniques that are employed
//right now are very primative

analyze::analyze(uint8_t *start_addr)
{
    this->func_info.func_start = start_addr;
    this->locate_end();
}

void analyze::locate_end()
{
    //find \xc9\xc3 (leave, ret)
    this->func_info.func_end = bstrstr(this->func_info.func_start, (uint8_t *)"\xc9\xc3", 400, 2) + 2;
    if(this->func_info.func_end == NULL){
        this->message = (char *)"End of function not found\nDisasmbling first 50 bytes";
        this->func_info.func_end = this->func_info.func_start + 50;
    }else{
        snprintf(buf, sizeof(this->buf), "Function length auto-detected to %u bytes", (unsigned)(this->func_info.func_end - this->func_info.func_start + 1));
        this->message = this->buf;
    }
}

size_t analyze::len()
{
    return this->func_info.func_end - this->func_info.func_start;
}
