#ifndef _PHYSICSPROCSSORBUILDER_H
#define _PHYSICSPROCSSORBUILDER_H

#include "SubstanceCollector.h"
#include "StructTree.h"
#include "KernelCollector.h"
#include "KernelQueueBuilder.h"


class PhysicsProcessorBuilder{
    public:
        PhysicsProcessorBuilder();
        ~PhysicsProcessorBuilder();

        char parseSystemConfig(std::string path);//reads configuration of entire system, no need to call other set functions

        char setKernelConfigFilePath(std::string path);
        char setMacroConfigFilePath(std::string path);
        char setSubstanceConfigFilePath(std::string path);
        char setStructDirAndRootFile(std::string dir, std::string rootFile);

        void setClPlatform(cl_platform_id platform);
        void setClDevice(cl_device_id device);

        // char setPBO(GLuit PBO);
        std::string getError();
        std::string translateClBuildError(cl_int error);

        char build();

        // PhysicsProcessor* build();

    private:
        StructTree* structTree;
        SizeCalculator* sizeCalculator;
        MacroManager* macroManager;
        ClStructParser* clParser;
        KernelCollector* kernelCollector;
        KernelQueueBuilder* kernelQueueBuilder;
        SubstanceCollector* substanceCollector;
        std::string error;     

        std::string kernelConfigFilePath; 
        std::string macroConfigFilePath;
        std::string substanceConfigFilePath;
        std::string structDir;
        std::string structRootFile;

        cl_platform_id clPlatformID;
        cl_device_id clDeviceID;

        // GLuit PBO;

        char parseConfigFiles(); //reads location and priority of kernels, root location of structs, macros, location of substances directory
        char createClContext(); //creates cl context (platform / device) based on config file
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
