#ifndef SIZE_CALCULATOR_H
#define SIZE_CALCULATOR_H

#include "engineStruct.h"

#include "../Headers.h"

class SizeCalculator{
public:
    SizeCalculator(uint pointerSize);

    char calculate(engineStruct* structure);
    ushort clTypeSize(engineStruct::cl_type type);

    std::string getError();

private:
    const uint pointerSize;

    std::string error;
};

#endif
