#ifndef MACRO_H
#define MACRO_H

#include <string>

struct macro {
    enum macro_type {
        SET,
        ALLOCATE
    };
    macro(){}
    macro(macro_type type, float value): type(type), value(value){}
    
    macro_type type;
    float value;
};

#endif
