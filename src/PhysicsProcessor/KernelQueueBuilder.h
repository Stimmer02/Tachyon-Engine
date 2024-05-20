#ifndef KERNELQUEUEBUILDER_H
#define KERNELQUEUEBUILDER_H

#include "kernelExecutionUnit.h"

#include <fstream>
#include <vector>

class KernelQueueBuilder{
    public:
        KernelQueueBuilder();

        char parseConfig(std::string path);
        std::vector<kernelExecutionUnit>& getKernelQueue();
        std::string getError();
    private:
        std::vector<kernelExecutionUnit> kernelQueue;

        std::string error;
};

#endif