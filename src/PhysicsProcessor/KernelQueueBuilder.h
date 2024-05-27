#ifndef KERNELQUEUEBUILDER_H
#define KERNELQUEUEBUILDER_H

#include "kernelExecutionUnit.h"
#include "KernelCollector.h"

#include <fstream>
#include <vector>

class KernelQueueBuilder{
    public:
        KernelQueueBuilder();

        char parseConfig(std::string path);
        char collectKernels(KernelCollector& kc);
        const std::vector<kernelExecutionUnit>& getKernelQueue();
        uint getKernelQueueSize();
        std::string getError();
    private:
        std::vector<kernelExecutionUnit> kernelQueue;

        std::string error;
};

#endif