#include "algs.h"

//all of the algoritms here are well thought out and
//safe, fast, and correctly coded

uint8_t *bstrstr(uint8_t *data, uint8_t *pattern, size_t data_len, size_t pattern_len)
{
    uint8_t *it = data;
    unsigned inc = 0;

    while(it < data + data_len){
        while(*(it + inc) == *(pattern + inc)){
            inc++;

            if(inc >= pattern_len)
                return it;

            if(it + inc >= data + data_len)
                return NULL;
        }
        inc = 0;
        it++;
    }
    return NULL;
}
