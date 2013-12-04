#include "wrap.h"

#include <cstdint>
#include <cassert>
#include <cstdlib>
#include <cstring>

using namespace std;

//TODO: allow get all c primatives

//fixme:have a memory leak(auto_ptr can solve this easily)
uint64_t string_to_char_ptr(string str)
{
    char *ret = (char *)malloc(strlen(str.c_str()));
    assert(ret);
    strcpy(ret, str.c_str());
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


char *wrap(vector<string> *args, vector<string> *types, void *fp)
{
    char *(*real_fp)() = (char *(*)())fp;
 
    if(args->size() != types->size()){
        return (char *) "invalid call";
    }    
 
    vector<uint64_t> converted;

    //fixme: not use this(need a deque)
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
    
    return (real_fp)();
}