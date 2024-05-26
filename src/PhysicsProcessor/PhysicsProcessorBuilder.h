#ifndef PHYSICSPROCSSORBUILDER_H
#define PHYSICSPROCSSORBUILDER_H

#include "SubstanceCollector.h"
#include "StructTree.h"
#include "KernelCollector.h"
#include "KernelQueueBuilder.h"
#include "../ECSystem/Configurator.h"

#include "PhysicsProcessor.h"

#include <format>



class PhysicsProcessorBuilder{
public:
    PhysicsProcessorBuilder();
    ~PhysicsProcessorBuilder();

    char parseSystemConfig(std::string path);//reads configuration of entire system, no need to call other set functions

    char setKernelConfigFilePath(std::string path);
    char setMacroConfigFilePath(std::string path);
    char setSubstanceConfigFilePath(std::string path);
    char setStructDirAndRootFile(std::string dir, std::string rootFile);
    char setConfigStructFile(std::string path);
    void setClPlatformAndDevice(cl_uint platform, cl_uint device);
    void setLocalWorkSize(cl::NDRange localWorkSize);

    void setClPlatform(cl_uint platform);
    void setClDevice(cl_uint device);

    void setPBO(GLuint PBO);

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
    std::string configStructFile;

    engineStruct* configStructure;

    cl_uint clPlatformID;
    cl_uint clDeviceID;
    std::string clDeviceName;
    cl::Program program;

    uint simWidth;
    uint simHeight;

    cl::NDRange localWorkSize;

    GLuint PBO;

    char parseConfigFiles(); //reads location and priority of kernels, root location of structs, macros, location of substances directory
    char createClContext(); //creates cl context (platform / device) based on config file
    char compileCl(); //compiles all the kernels and structs
    char createSubstanceStructure(); //creates substance structure based on loaded substances
    char buildStructTree(); //builds struct tree and calculates its properties
    char addConfigStructure(); //adds config structure to openCL structrures and calculates its properties
    char loadKernels(); //loads kernels from files
    void createPhysicsProcessor(); //creates PhysicsProcessor object
    void addMandatoryKernels(); //adds kernels that are mandatory for PhysicsProcessor to work
    char checkLocalWorkSize(); //checks if local work size is valid
    char setMandatoryKernels(); //sets previously defined kernels
    char setKernelQueue(); //sets engine kernel queue based on their priority
    char allocateGPUResourcesMemory(); //allocates memory for all structures based on structure tree
    char allocateStructure(const engineStruct* structure, const std::map<std::string, cl::Kernel>& kernels, cl::Buffer& buffer, uint count = 1); //allocates memory and sets structure hierarchy recursively
    std::string createAllocationKernel(const engineStruct* structure); //creates kernel that allocates memory for particullar structure
    char allocateGPUConfigStructure(); //allocates and initializes memory for config structure
    std::string createConfigStructureKernel(); //creates config structure based on config file
    char acquireGlObjectFromPBO(); //acquires gl object from PBO
};

#endif
