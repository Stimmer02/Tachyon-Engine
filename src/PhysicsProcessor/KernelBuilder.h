#ifndef _KERNELBUILDER_H
#define _KERNELBUILDER_H

#include "PhysicsProcessor.h"
#include "kernelFragment.h"

#include <vector>
#include <algorithm>


class KernelBuilder{
    // friend class PhysicsEngineBuilder;
public:
    char loadAllFromDirectory(const std::string direcory);
    char addKernelFragment(const kernelFragment& kernelFragment);
    char addKernelFragment(const std::string filePath);

    char validateLoadedFragments(std::string& outMessage);
    std::string to_string();

    std::string build(cl::Program::Sources& sources);

private:
    std::vector<kernelFragment> fragments;
};

#endif
