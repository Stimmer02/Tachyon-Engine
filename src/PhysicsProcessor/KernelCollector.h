#ifndef KERNELCOLLECTOR_H
#define KERNELCOLLECTOR_H

#include "kernelData.h"

#include <vector>

class KernelCollector{
public:
    KernelCollector();
    ~KernelCollector(); 

    char addKernel(std::string path, uint priority);
    std::string getKernels(); //returns kernels code in a single string

private:
    std::vector<kernelData> kernels;

    char checkKernel(std::string* kernel); //checks if kernel has proper signature
};
#endif
