#ifndef PHYSICSPROCESSORBUILDER_H
#define PHYSICSPROCESSORBUILDER_H

#include "PhysicsProcessor.h"
#include "ClStructParser.h"
#include "StructTree.h"
#include "SizeCalculator.h"
#include "MacroManager.h"
#include "KernelCollector.h"
#include "SubstanceCollector.h"

class PhysicsProcessorBuilder{
    public:
        PhysicsProcessorBuilder();
        ~PhysicsProcessorBuilder();

        char setConfigFilePath(std::string path);
        char setPBO(GLuit PBO);
        std::string getError();

        PhysicsProcessor* build();

    private:
        StructTree* structTree;
        SizeCalculator* sizeCalculator;
        MacroManager* macroManager;
        KernelCollector* kernelCollector;
        std::string error;     
        std::string configFilePath; 
        GLuit PBO;

        char parseConfigFile(std::string path); //reads location and priority of kernels, root location of structs, macros, location of substances directory
        char buildStructTree(); //builds struct tree and calculates its properties
        char compileCl(); //compiles all the kernels and structs
};


#endif