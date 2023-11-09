#ifndef _KERNELFRAGMENT_H
#define _KERNELFRAGMENT_H

#include <string>
#include <filesystem>
#include <fstream>

struct kernelFragment{
    kernelFragment(const int priority, const std::string rawKernel, std::string name);
    kernelFragment(const std::string filePath);

    static bool comp(const kernelFragment& a, const kernelFragment& b);

    int priority;
    std::string rawKernel;
    std::string name;
};
/*
positive priories will be added in ascending order
negative priories will be added after positive also in ascending order (so it is possible to add fragment at the end of the kernel)
priorities that repeat will be ordered at random
0 and -1 are prohibited to use
*/
#endif
