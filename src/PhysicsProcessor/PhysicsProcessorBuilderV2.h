#ifndef PHYSICSPROCESSORBUILDER_H
#define PHYSICSPROCESSORBUILDER_H

#include "PhysicsProcessor.h"
#include "ClStructParser.h"
#include "StructTree.h"
#include "SizeCalculator.h"
#include "MacroManager.h"
#include "KernelCollector.h"
#include "SubstanceCreator.h"

class PhysicsProcessorBuilder{
    public:
        PhysicsProcessorBuilder();
        ~PhysicsProcessorBuilder();

        char setConfigFilePath(std::string path);
        char setPBO(GLuit PBO);
        std::string getError();
        std::string translateClBuildError(cl_int error);

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
        char createClCotent(); //creates cl content (platform device) based on config file
        char buildStructTree(); //builds struct tree and calculates its properties
        char compileCl(); //compiles all the kernels and structs
        char addMandatoryKernels(); //adds kernels that are mandatory for PhysicsProcessor to work
        char createSubstanceTable(); //creates substance table
        char compileCl(); //compiles all the kernels and structs
        char setKernelQueue(); //sets engine kernel queue based on their priority
        char setMandatoryKernels(); //sets previously defined kernels
        std::string createAllocationKernel(); //creates kernel that allocates memory for particullar structure
        char allocateGPUMemory(); //allocates memory for all structures based on structure tree
        char acquireGlObjectFromPBO(); //acquires gl object from PBO
        char createClQueue(); //creates cl queue
};


#endif