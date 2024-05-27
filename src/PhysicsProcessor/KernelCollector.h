#ifndef KERNELCOLLECTOR_H
#define KERNELCOLLECTOR_H

#include "kernelData.h"

#include <vector>
#include <string>
#include <fstream>

class KernelCollector{
public:
    KernelCollector();

    char addKernel(std::string path, std::string& kernelName);
    char addKernelCode(const std::string& code, const std::string& kernelName);
    std::string getKernels(); //returns kernels code in a single string
    std::string getError();

private:
    std::vector<kernelData> kernels;

    std::string error;

    char checkKernel(std::string* kernel, std::string& kernelName); //checks if kernel has proper signature and returns kernel name
};
#endif
