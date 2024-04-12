#ifndef KERNELDATA_H
#define KERNELDATA_H

#include <string>

struct kernelData{
    kernelData(std::string kernel, uint priority, std::string name) : kernel(kernel), priority(priority), name(name){}

    std::string kernel;
    uint priority;
    std::string name;
};

#endif