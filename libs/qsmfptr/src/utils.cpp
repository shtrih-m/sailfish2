#include "utils.h"

bool testBit(uint64_t value, int bit){
    return (value & (1 << bit)) != 0;
};

