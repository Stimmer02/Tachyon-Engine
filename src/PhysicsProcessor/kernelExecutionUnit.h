#ifndef KERNELEXECUTIONUNIT_H
#define KERNELEXECUTIONUNIT_H

#include <string>
#include <stdint.h>

struct kernelExecutionUnit{
    kernelExecutionUnit(std::string name, std::string path, uint32_t executionCount){
        this->name = name;
        this->path = path;
        this->executionCount = executionCount;

        functionName.clear();
    }

    std::string name; //name in configuraion file
    std::string functionName; //name in code
    std::string path;
    uint32_t executionCount;
};

#endif
