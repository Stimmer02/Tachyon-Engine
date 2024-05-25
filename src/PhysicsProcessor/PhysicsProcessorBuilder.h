#ifndef PHYSICSPROCSSORBUILDER_H
#define PHYSICSPROCSSORBUILDER_H

#include "SubstanceCollector.h"
#include "StructTree.h"
#include "KernelCollector.h"
#include "KernelQueueBuilder.h"
#include "../ECSystem/Configurator.h"

#include "PhysicsProcessor.h"



class PhysicsProcessorBuilder{
public:
    PhysicsProcessorBuilder();
    ~PhysicsProcessorBuilder();

    char parseSystemConfig(std::string path);//reads configuration of entire system, no need to call other set functions

    char setKernelConfigFilePath(std::string path);
    char setMacroConfigFilePath(std::string path);
    char setSubstanceConfigFilePath(std::string path);
    char setStructDirAndRootFile(std::string dir, std::string rootFile);
    void setClPlatformAndDevice(cl_uint platform, cl_uint device);
    void setLocalWorkSize(cl::NDRange localWorkSize);

    void setClPlatform(cl_uint platform);
    void setClDevice(cl_uint device);

    // char setPBO(GLuit PBO);

    std::string getDeviceName();
    std::string getError();
    std::string translateClBuildError(cl_int error);

    char build();
    PhysicsProcessor* getPhysicsProcessor();

private:
    PhysicsProcessor* physicsProcessor;

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

    cl_uint clPlatformID;
    cl_uint clDeviceID;
    std::string clDeviceName;
    cl::Program program;

    uint simWidth;
    uint simHeight;

    cl::NDRange globalWorkSize;
    cl::NDRange localWorkSize;

    // GLuit PBO;

    char parseConfigFiles(); //reads location and priority of kernels, root location of structs, macros, location of substances directory
    char createClContext(); //creates cl context (platform / device) based on config file
    char createSubstanceStructure(); //creates substance structure based on loaded substances
    char buildStructTree(); //builds struct tree and calculates its properties
    char loadKernels(); //loads kernels from files
    void addMandatoryKernels(); //adds kernels that are mandatory for PhysicsProcessor to work
    char compileCl(); //compiles all the kernels and structs
    char setMandatoryKernels(); //sets previously defined kernels
    char setKernelQueue(); //sets engine kernel queue based on their priority
    std::string createAllocationKernel(); //creates kernel that allocates memory for particullar structure
    char allocateGPUMemory(); //allocates memory for all structures based on structure tree
    char acquireGlObjectFromPBO(); //acquires gl object from PBO
    char createClQueue(); //creates cl queue
};

#endif
