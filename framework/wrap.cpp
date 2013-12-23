#include "framework.h"

#include <iostream>

using std::cout;
using std::endl;

using std::unordered_map;
using std::string;
using std::vector;

namespace framework
{

namespace convert
{

static vector<void *> free_list;

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

void str_free()
{
    for(void *tmp : free_list){
        free(tmp);
    }
}

//the functions to convert types
static unordered_map<string, void *> lookup = {
{"unsigned", (void *)&string_to_unsigned},
{"const char *", (void *)&string_to_char_ptr}
};

} //end namespace convert

wrap::wrap()
{

}

wrap::~wrap()
{

}

//TODO this return should be more flexible
char *wrap::call_i(vector<string> *args, vector<string> *types, FARPROC fp)
{
    cout << "call_i" << endl;

    char *(*real_fp)() = (char *(*)())fp;
 
    if(args->size() != types->size()){
        return (char *) "invalid call";
    }    
 
    vector<uint64_t> converted;

    //fixme: not use this(need a deque) <- what was I talking about????
    int cnt = 0;
    
    for(string str : *types){
        auto it = convert::lookup.find(str);
        if(it == convert::lookup.end()){
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
            return (char *)"fixme:too many arguments(more than four)";
    }
    
    char *ret = (real_fp)();
    
    //this assumes that the function does not return a string it was passed
    //otherwise it will break since the strings that it was passed were just
    //freed
    convert::str_free();
    
    return ret;
}

char *wrap::call(string input)
{
    cout << "wrap::call with " << input.c_str() << endl << std::flush;

    char *buf = (char *)malloc(input.length() + 1);
    strcpy(buf, input.c_str());

    string module(strtok(buf, ":"));

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

    FARPROC fp = dll_utils::address(module, func);

    func.append("TYPE");

    const char *types_raw = *(const char **)dll_utils::address(module, func);    
    
    char *types_raw_cpy = (char *)malloc(strlen(types_raw) + 1);
    strcpy(types_raw_cpy, types_raw);

    vector<string> types;

    if(strlen(types_raw_cpy) > 1){
        types.push_back(strtok(types_raw_cpy, ","));

        char *it = NULL;

        while((it = strtok(NULL, ","))){
            types.push_back(string(it));
        }
    }

    cout << "types:" << endl;
    for( string tmpstr : types){
        cout << tmpstr.c_str() << endl;
    }
    
    cout << "args:" << endl;
    for( string tmpstr : args){
        cout << tmpstr.c_str() << endl;
    }
    
    char *ret = this->call_i(&args, &types, fp);
    
    cout << "here" << endl;
    
    free(types_raw_cpy);
    
    return ret;
}

}