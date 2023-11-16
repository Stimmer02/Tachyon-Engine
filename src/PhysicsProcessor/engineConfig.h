#ifndef _ENGINECONFIG_H
#define _ENGINECONFIG_H

#include <CL/opencl.hpp>
typedef unsigned int uint;

struct engineConfig{
    uint simulationWidth;
    uint simulationHeight;
    float gravity;
    float timefactor;
    float atmosphereViscosity;
};

#endif
