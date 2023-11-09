#ifndef _STRUCTCOLLECTOR_H
#define _STRUCTCOLLECTOR_H

#include "PhysicsProcessor.h"

#include <fstream>
#include <filesystem>

class StructCollector{
public:
    char loadAllFromDirectory(const std::string direcory);
    char addStruct(const std::string filePath);

    cl::Program::Sources get();
    void get(cl::Program::Sources& sources);

private:
    std::string structures;
};

#endif
