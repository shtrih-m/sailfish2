#include "utils.h"

bool testBit(int value, int bit){
    return (value & (1 << bit)) != 0;
};

