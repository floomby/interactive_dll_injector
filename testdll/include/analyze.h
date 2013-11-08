#ifndef ANALYZE_H
#define ANALYZE_H

#include <cstdint>

#include "../algs.h"

class analyze
{
    public:
        analyze(uint8_t *start_addr);
        size_t len();
    protected:
    private:
        void locate_end();
        struct {
            uint8_t *func_start;
            uint8_t *func_end;
        } func_info;
};

#endif // ANALYZE_H
