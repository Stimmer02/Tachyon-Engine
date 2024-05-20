#ifndef KERNELQUEUEBUILDER_H
#define KERNELQUEUEBUILDER_H

#include "kernelExecution.h"
#include <string>

class KernelQueueBuilder{
    public:
        KernelQueueBuilder();
        ~KernelQueueBuilder();

        char parseConfig(std::string path); //parses config file with kernels

        kernelExecution* getKernelQueue();

    private:
        uint kernelCount;
        kernelExecution* kernelQueue;
};

#endif