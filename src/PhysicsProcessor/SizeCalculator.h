#ifndef SIZE_CALCULATOR_H
#define SIZE_CALCULATOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#include "engineStruct.h"

#ifdef __APPLE__

#include <OpenCL/opencl.h>
#include "../OpenCL/include/CL/cl.hpp"

#elif __WIN32__

typedef unsigned int uint;

#include <CL/opencl.hpp>

#else

#include <CL/opencl.hpp>

#endif

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
