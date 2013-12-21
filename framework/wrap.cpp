#include "framework.h"

#include "wrap.h"

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>

using namespace std;

//TODO: allow get all c primatives

namespace framework
{

vector<void *> free_list;

uint64_t string_to_char_ptr(string str)
{
    char *ret = (char *)malloc(str.length() + 1);
    strcpy(ret, str.c_str());
    free_list.push_back(ret);
    return (uint64_t)ret;
};
uint64_t string_to_unsigned(string str){ return (uint64_t)atoi(str.c_str()); }
uint64_t string_to_unsignedlong(string str){ return (uint64_t)strtoul(str.c_str(), NULL, 0); }
uint64_t string_to_unsignedlonglong(string str){ return strtoull(str.c_str(), NULL, 0); }

//the functions to convert types
static unordered_map<string, void *> lookup = {
{"unsigned", (void *)&string_to_unsigned},
{"const char *", (void *)&string_to_char_ptr}
};

//TODO this return should be more flexible
char *wrap(vector<string> *args, vector<string> *types, void *fp)
{
    char *(*real_fp)() = (char *(*)())fp;
 
    if(args->size() != types->size()){
        return (char *) "invalid call";
    }    
 
    vector<uint64_t> converted;

    //fixme: not use this(need a deque) <- what was I talking about????
    int cnt = 0;
    
    for(string str : *types){
        auto it = lookup.find(str);
        if(it == lookup.end()){
            return (char *) "invalid type";
        }
        uint64_t (*conv_fp)(string) = (uint64_t (*)(string))it->second;
        converted.push_back((conv_fp)(args->at(cnt)));
        cnt++;
    }
 
    switch(converted.size()){
        case 4:
        __asm__ __volatile__(
                "movq %0, %%rcx;"
                "movq %1, %%rdx;"
                "movq %2, %%r8;"
                "movq %2, %%r9;"
                : /* no outputs */
                :   "r" (converted.at(0)),
                    "r" (converted.at(1)),
                    "r" (converted.at(2)),
                    "r" (converted.at(3))
                : "rcx", "rdx", "r8", "r9"
        );
        break;
    
        case 3:
        __asm__ __volatile__(
                "movq %0, %%rcx;"
                "movq %1, %%rdx;"
                "movq %2, %%r8;"
                : /* no outputs */
                :   "r" (converted.at(0)),
                    "r" (converted.at(1)),
                    "r" (converted.at(2))
                : "rcx", "rdx", "r8"
        );
        break;
        
        case 2:
        __asm__ __volatile__(
                "movq %0, %%rcx;"
                "movq %1, %%rdx;"
                : /* no outputs */
                :   "r" (converted.at(0)),
                    "r" (converted.at(1))
                : "rcx", "rdx"
        );
        break;
        
        case 1:
        __asm__ __volatile__(
                "movq %0, %%rcx;"
                : /* no outputs */
                : "r" (converted.at(0))
                : "rcx"
        );
        break;
        
        case 0:
            break;

        default:
            return (char *)"fixme:too many arguments";
    }
    
    for(void *it : free_list){
        free(it);
    }
    
    return (real_fp)();
}

void call(char *cmd, unordered_map<string, string> *dllMap)
{
    string module(strtok(cmd, ":"));

    string func(strtok(NULL, "("));

    char *args_raw = strtok(NULL, ")");

    vector<string> args;

    if(args_raw){
        args.push_back(strtok(args_raw, ","));

        char *it = NULL;

        while((it = strtok(NULL, ","))){
            args.push_back(string(it));
        }
    }

    if(!dllMap->count(module)){
        if(!load(module, dllMap)){
            erl_send("loading module");
        }else{
            erl_send("failed to  load module");
            return;
        }
    }

    void *fp = (void *)GetProcAddress(GetModuleHandle(module.c_str()), func.c_str());

    func.append("TYPE");

    const char *types_raw = *(const char **)GetProcAddress(GetModuleHandle(module.c_str()), func.c_str());    
    
    char *types_raw_cpy = (char *)malloc(strlen(types_raw));
    assert(types_raw_cpy);
    strcpy(types_raw_cpy, types_raw);

    vector<string> types;

    if(strlen(types_raw_cpy) > 1){
        types.push_back(strtok(types_raw_cpy, ","));

        char *it = NULL;

        while((it = strtok(NULL, ","))){
            types.push_back(string(it));
        }
    }

    char *ret = wrap(&args, &types, fp);

    erl_send(ret);

    free(types_raw_cpy);
    free(ret);
}

bool load(string module, unordered_map<string, string> *dllMap)
{
    char cwd_buf[1024];
    string dllName(_getcwd(cwd_buf, sizeof(cwd_buf)));
    
    dllName += "\\modules\\" + module + "\\" + module + ".dll";
    
    if(!LoadLibraryA(dllName.c_str())){
        return true;
    }
    
    dllMap->insert(make_pair<string, string>(module.c_str(), dllName.c_str()));
    return false;
}

}