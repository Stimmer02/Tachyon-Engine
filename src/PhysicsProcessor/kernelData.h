#ifndef KERNELDATA_H
#define KERNELDATA_H

#include <string>

struct kernelData{
    kernelData(std::string kernel, std::string name) : kernel(kernel), name(name){}

    std::string kernel;
    std::string name;
};

#endif