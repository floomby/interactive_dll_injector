#include "framework.h"

using std::unordered_map;
using std::make_pair;
using std::string;

namespace framework
{

namespace dll_utils
{

static unordered_map<string, HMODULE> loaded;

void load(string module)
{
    //TODO is this big enough
    char cwd_buf[1024];
    string dllName(_getcwd(cwd_buf, sizeof(cwd_buf)));
    
    dllName += "\\modules\\" + module + "\\" + module + ".dll";
    
    HMODULE hMod;
    
    if(!(hMod = LoadLibraryA(dllName.c_str()))){
        throw "unable to loaad library";
    }
    
    loaded.insert(make_pair(string(module), hMod));
}

FARPROC address(string module, string function)
{
    auto it = loaded.find(module);

    if(it == loaded.end()){
        load(module);
        return address(module, function);
    }
    
    return GetProcAddress(it->second, function.c_str());
}

} // end namespace dll_utils

} // end namespace framework