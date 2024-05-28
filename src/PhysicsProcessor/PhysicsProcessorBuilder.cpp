#include "PhysicsProcessorBuilder.h"

PhysicsProcessorBuilder::PhysicsProcessorBuilder(){
    physicsProcessor = nullptr;
    defaultToFallback = false;

    structTree = new StructTree();
    sizeCalculator = nullptr;
    macroManager = new MacroManager();
    clParser = new ClStructParser(macroManager);
    kernelCollector = new KernelCollector();
    kernelQueueBuilder = new KernelQueueBuilder();
    substanceCollector = new SubstanceCollector();

    error = "";
    kernelConfigFilePath = "";
    macroConfigFilePath = "";
    substanceConfigFilePath = "";
    structDir = "";
    structRootFile = "";
    configStructFile = "";

    clPlatformID = 0;
    clDeviceID = 0;

    simWidth = 0;
    simHeight = 0;

    configStructure = nullptr;
    clErrorFunction = nullptr;

    clDeviceName = "";

    localWorkSize = cl::NDRange(0);

    TBO = 0;
}

PhysicsProcessorBuilder::~PhysicsProcessorBuilder(){
    delete structTree;
    delete clParser;
    delete macroManager;
    delete kernelCollector;
    delete kernelQueueBuilder;
    delete substanceCollector;

    if (sizeCalculator != nullptr){
        delete sizeCalculator;
    }

    if (physicsProcessor != nullptr){
        delete physicsProcessor;
    }

    if (configStructure != nullptr){
        delete configStructure;
    }

    // if (clErrorFunction != nullptr){ probably handeled higher
    //     delete clErrorFunction;
    // }
}


char PhysicsProcessorBuilder::parseSystemConfig(std::string path){
    Configurator config(path);

    std::string temp, rootStructureTemp;

    config.ParseString("KERNEL_CONFIG_PATH", temp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig KERNEL_CONFIG_PATH not found\n";
        return 1;
    }
    if (setKernelConfigFilePath(temp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set kernel config file path\n";
        return 1;
    }

    config.ParseString("MACRO_CONFIG_PATH", temp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig MACRO_CONFIG_PATH not found\n";
        return 2;
    }
    if (setMacroConfigFilePath(temp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set macro config file path\n";
        return 2;
    }

    config.ParseString("SUBSTANCE_CONFIG_PATH", temp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig SUBSTANCE_CONFIG_PATH not found\n";
        return 3;
    }
    if (setSubstanceConfigFilePath(temp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set substance config file path\n";
        return 3;
    }

    config.ParseString("STRUCT_DIR", temp, "");
    config.ParseString("STRUCT_ROOT_FILE", rootStructureTemp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig STRUCT_DIR not found\n";
        return 4;
    }
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig STRUCT_ROOT_FILE not found\n";
        return 5;
    }
    if (setStructDirAndRootFile(temp, rootStructureTemp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set struct dir and root file\n";
        return 5;
    }

    config.ParseString("CONFIG_STRUCT_FILE", temp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig CONFIG_STRUCT_FILE not found\n";
        return 6;
    }
    if (setConfigStructFile(temp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set config struct file\n";
        return 6;
    }

    config.ParseString("MANDATORY_KERNELS_DIR", temp, "");
    if (temp == ""){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig MANDATORY_KERNELS_FILE not found\n";
        return 7;
    }
    if (setMandatoryKernelsDir(temp) != 0){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig failed to set mandatory kernels file\n";
        return 7;
    }
    
    bool fallback;
    config.ParseBoolean("DEFAULT_TO_FALLBACK", fallback, false);
    setDefaultToFallback(fallback);

    int platform;
    int device;

    config.ParseInt("CL_PLATFORM_ID", platform, -1);
    config.ParseInt("CL_DEVICE_ID", device, -1);
    if (platform == -1 || device == -1){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig CL_PLATFORM_ID or CL_DEVICE_ID not found\n";
        return 8;
    }
    setClPlatformAndDevice(platform, device);

    int localWorkSizeX;
    int localWorkSizeY;

    config.ParseInt("LOCAL_WORK_SIZE_X", localWorkSizeX, 0);
    config.ParseInt("LOCAL_WORK_SIZE_Y", localWorkSizeY, 0);
    setLocalWorkSize(cl::NDRange(localWorkSizeX, localWorkSizeY));

    return 0;
}


char PhysicsProcessorBuilder::setKernelConfigFilePath(std::string path){
    std::ifstream file(path);
    if (file.good() && file.is_open()) {
        kernelConfigFilePath = path;
        return 0;
    } else {
        error += "ERR: PhysicsProcessorBuilder::setKernelConfigFilePath file '"+ path +"' not found\n";
        return -1;
    }
}

char PhysicsProcessorBuilder::setMacroConfigFilePath(std::string path){
    std::ifstream file(path);
    if (file.good() && file.is_open()) {
        macroConfigFilePath = path;
        return 0;
    } else {
        error += "ERR: PhysicsProcessorBuilder::setMacroConfigFilePath file '"+ path +"' not found\n";
        return -1;
    }
}

char PhysicsProcessorBuilder::setSubstanceConfigFilePath(std::string path){
    std::ifstream file(path);
    if (file.good() && file.is_open()) {
        substanceConfigFilePath = path;
        return 0;
    } else {
        error += "ERR: PhysicsProcessorBuilder::setSubstanceConfigFilePath file '"+ path +"' not found\n";
        return -1;
    }
}

char PhysicsProcessorBuilder::setStructDirAndRootFile(std::string dir, std::string rootFile){
    structDir = dir;
    structRootFile = rootFile;
    return 0;
}

char PhysicsProcessorBuilder::setConfigStructFile(std::string path){
    std::ifstream file(path);
    if (file.good() && file.is_open()) {
        configStructFile = path;
        return 0;
    } else {
        error += "ERR: PhysicsProcessorBuilder::setConfigStructFile file '"+ path +"' not found\n";
        return -1;
    }
}

char PhysicsProcessorBuilder::setMandatoryKernelsDir(std::string path){
    std::ifstream file(path);
    if (file.good() && file.is_open()) {
        mandatoryKernelsDir = path;
        return 0;
    } else {
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernelsDir file '"+ path +"' not found\n";
        return -1;
    }
}

void PhysicsProcessorBuilder::setClPlatformAndDevice(cl_uint platform, cl_uint device){
    clPlatformID = platform;
    clDeviceID = device;
}

void PhysicsProcessorBuilder::setLocalWorkSize(cl::NDRange localWorkSize){
    this->localWorkSize = localWorkSize;
}

void PhysicsProcessorBuilder::setDefaultToFallback(bool fallback){
    defaultToFallback = fallback;
}


void PhysicsProcessorBuilder::setClPlatform(cl_uint platform){
    clPlatformID = platform;
}

void PhysicsProcessorBuilder::setClDevice(cl_uint device){
    clDeviceID = device;
}

void PhysicsProcessorBuilder::setClErrorFunction(std::function<void(std::string)>* errorFunction){
    clErrorFunction = errorFunction;
}

void PhysicsProcessorBuilder::setTBO(GLuint TBO){
    this->TBO = TBO;
}

std::string PhysicsProcessorBuilder::getDeviceName(){
    return clDeviceName;
}


std::string PhysicsProcessorBuilder::getError(){
    return error;
}

std::string PhysicsProcessorBuilder::translateClBuildError(cl_int error){
    switch(error){
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

        // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

        // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

std::string PhysicsProcessorBuilder::translateClError(cl_int error){
    switch(error){
        case CL_SUCCESS: return "CL_SUCCESS";
        case CL_DEVICE_NOT_FOUND: return "CL_DEVICE_NOT_FOUND";
        case CL_DEVICE_NOT_AVAILABLE: return "CL_DEVICE_NOT_AVAILABLE";
        case CL_COMPILER_NOT_AVAILABLE: return "CL_COMPILER_NOT_AVAILABLE";
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case CL_OUT_OF_RESOURCES: return "CL_OUT_OF_RESOURCES";
        case CL_OUT_OF_HOST_MEMORY: return "CL_OUT_OF_HOST_MEMORY";
        case CL_PROFILING_INFO_NOT_AVAILABLE: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case CL_MEM_COPY_OVERLAP: return "CL_MEM_COPY_OVERLAP";
        case CL_IMAGE_FORMAT_MISMATCH: return "CL_IMAGE_FORMAT_MISMATCH";
        case CL_IMAGE_FORMAT_NOT_SUPPORTED: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case CL_BUILD_PROGRAM_FAILURE: return "CL_BUILD_PROGRAM_FAILURE";
        case CL_MAP_FAILURE: return "CL_MAP_FAILURE";
        case CL_MISALIGNED_SUB_BUFFER_OFFSET: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case CL_COMPILE_PROGRAM_FAILURE: return "CL_COMPILE_PROGRAM_FAILURE";
        case CL_LINKER_NOT_AVAILABLE: return "CL_LINKER_NOT_AVAILABLE";
        case CL_LINK_PROGRAM_FAILURE: return "CL_LINK_PROGRAM_FAILURE";
        case CL_DEVICE_PARTITION_FAILED: return "CL_DEVICE_PARTITION_FAILED";
        case CL_KERNEL_ARG_INFO_NOT_AVAILABLE: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
        case CL_INVALID_VALUE: return "CL_INVALID_VALUE";
        case CL_INVALID_DEVICE_TYPE: return "CL_INVALID_DEVICE_TYPE";
        case CL_INVALID_PLATFORM: return "CL_INVALID_PLATFORM";
        case CL_INVALID_DEVICE: return "CL_INVALID_DEVICE";
        case CL_INVALID_CONTEXT: return "CL_INVALID_CONTEXT";
        case CL_INVALID_QUEUE_PROPERTIES: return "CL_INVALID_QUEUE_PROPERTIES";
        case CL_INVALID_COMMAND_QUEUE: return "CL_INVALID_COMMAND_QUEUE";
        case CL_INVALID_HOST_PTR: return "CL_INVALID_HOST_PTR";
        case CL_INVALID_MEM_OBJECT: return "CL_INVALID_MEM_OBJECT";
        case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case CL_INVALID_IMAGE_SIZE: return "CL_INVALID_IMAGE_SIZE";
        case CL_INVALID_SAMPLER: return "CL_INVALID_SAMPLER";
        case CL_INVALID_BINARY: return "CL_INVALID_BINARY";
        case CL_INVALID_PROGRAM: return "CL_INVALID_PROGRAM";
        case CL_INVALID_PROGRAM_EXECUTABLE: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case CL_INVALID_KERNEL_NAME: return "CL_INVALID_KERNEL_NAME";
        case CL_INVALID_KERNEL_DEFINITION: return "CL_INVALID_KERNEL_DEFINITION";
        case CL_INVALID_KERNEL: return "CL_INVALID_KERNEL";
        case CL_INVALID_ARG_INDEX: return "CL_INVALID_ARG_INDEX";
        case CL_INVALID_ARG_VALUE: return "CL_INVALID_ARG_VALUE";
        case CL_INVALID_ARG_SIZE: return "CL_INVALID_ARG_SIZE";
        case CL_INVALID_KERNEL_ARGS: return "CL_INVALID_KERNEL_ARGS";
        case CL_INVALID_WORK_DIMENSION: return "CL_INVALID_WORK_DIMENSION";
        case CL_INVALID_WORK_GROUP_SIZE: return "CL_INVALID_WORK_GROUP_SIZE";
        case CL_INVALID_WORK_ITEM_SIZE: return "CL_INVALID_WORK_ITEM_SIZE";
        case CL_INVALID_GLOBAL_OFFSET: return "CL_INVALID_GLOBAL_OFFSET";
        case CL_INVALID_EVENT_WAIT_LIST: return "CL_INVALID_EVENT_WAIT_LIST";
        case CL_INVALID_EVENT: return "CL_INVALID_EVENT";
        case CL_INVALID_OPERATION: return "CL_INVALID_OPERATION";
        case CL_INVALID_GL_OBJECT: return "CL_INVALID_GL_OBJECT";
        case CL_INVALID_BUFFER_SIZE: return "CL_INVALID_BUFFER_SIZE";
        case CL_INVALID_MIP_LEVEL: return "CL_INVALID_MIP_LEVEL";
        case CL_INVALID_GLOBAL_WORK_SIZE: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case CL_INVALID_PROPERTY: return "CL_INVALID_PROPERTY";
        case CL_INVALID_IMAGE_DESCRIPTOR: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case CL_INVALID_COMPILER_OPTIONS: return "CL_INVALID_COMPILER_OPTIONS";
        case CL_INVALID_LINKER_OPTIONS: return "CL_INVALID_LINKER_OPTIONS";
        case CL_INVALID_DEVICE_PARTITION_COUNT: return "CL_INVALID_DEVICE_PARTITION_COUNT";
#ifndef __APPLE__
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case CL_PLATFORM_NOT_FOUND_KHR: return "CL_PLATFORM_NOT_FOUND_KHR";
#else
        case -69: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -70: return "CL_PLATFORM_NOT_FOUND_KHR";
#endif
        default: return "Unknown OpenCL error";

    }
}

char PhysicsProcessorBuilder::build(bool verbose){
    if (verbose)std::printf("Checking if all necessary variables are set\n");
    if (kernelConfigFilePath == ""){
        error += "ERR: PhysicsProcessorBuilder::build kernelConfigFilePath not set\n";
        return 1;
    }
    if (macroConfigFilePath == ""){
        error += "ERR: PhysicsProcessorBuilder::build macroConfigFilePath not set\n";
        return 2;
    }
    if (substanceConfigFilePath == ""){
        error += "ERR: PhysicsProcessorBuilder::build substanceConfigFilePath not set\n";
        return 3;
    }
    if (structDir == ""){
        error += "ERR: PhysicsProcessorBuilder::build structDir not set\n";
        return 4;
    }
    if (structRootFile == ""){
        error += "ERR: PhysicsProcessorBuilder::build structRootFile not set\n";
        return 5;
    }
    if (configStructFile == ""){
        error += "ERR: PhysicsProcessorBuilder::build configStructFile not set\n";
        return 6;
    }
    if (mandatoryKernelsDir == ""){
        error += "ERR: PhysicsProcessorBuilder::build mandatoryKernelsDir not set\n";
        return 7;
    }
    if (TBO == 0){
        error += "ERR: PhysicsProcessorBuilder::build TBO not set\n";
        return 8;
    }

    if (verbose)std::printf("Parsing config files\n");
    if (parseConfigFiles() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to parse config files\n";
        return 9;
    }

    if (verbose)std::printf("Loading kernels\n");
    if (loadKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to load kernels\n";
        return 10;
    }

    if (verbose)std::printf("Creating physics processor\n");
    createPhysicsProcessor();

    if (verbose)std::printf("Creating OpenCL context\n");
    if (createClContext() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create cl context\n";
        return 11;
    }

    if (verbose)std::printf("Checking local work size\n");
    if (checkLocalWorkSize() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed during local work size check\n";
        return 12;
    }

    if (verbose)std::printf("Creating substance structure\n");
    if (createSubstanceStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create substance structure\n";
        return 13;
    }

    if (verbose)std::printf("Building struct tree\n");
    if (buildStructTree() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to build struct tree\n";
        return 14;
    }

    if (verbose)std::printf("Adding config structure\n");
    if (addConfigStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to add config structure\n";
        return 15;
    }

    if (verbose)std::printf("Compiling OpenCL program\n");
    if (compileCl() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to compile OpenCL program\n";
        return 16;
    }

    if (verbose)std::printf("Acquiring GL object from TBO\n");
    if (acquireGlObjectFromTBO() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to acquire GL object from TBO\n";
        return 19;
    }

    if (verbose)std::printf("Allocating GPU resources memory\n");
    uint allocatedMemory = 0;
    if (allocateGPUResourcesMemory(allocatedMemory) != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to allocate GPU memory\n";
        return 20;
    }
    if (verbose)std::printf("    Allocated overall %.2fMB\n", float(allocatedMemory)/float(1024.0*1024.0));

    if (verbose)std::printf("Allocating GPU config structure\n");
    if (allocateGPUConfigStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to allocate GPU memory for config structure\n";
        return 21;
    }

    if (verbose)std::printf("Allocating rest of GPU buffers\n");
    if (allocateGPUWorkBuffers() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to allocate rest of GPU buffers\n";
        return 22;
    }

    if (verbose)std::printf("Setting mandatory kernels\n");
    if (setMandatoryKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set mandatory kernels\n";
        return 17;
    }

    if (verbose)std::printf("Setting kernel queue (engine)\n");
    if (setKernelQueue() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set kernel queue\n";
        return 18;
    }

    if (verbose)std::printf("Done\n");
    return 0;
}

PhysicsProcessor* PhysicsProcessorBuilder::getPhysicsProcessor(){
    if (physicsProcessor == nullptr){
        error += "ERR: PhysicsProcessorBuilder::getPhysicsProcessor physicsProcessor not created\n";
    }

    PhysicsProcessor* out = physicsProcessor;
    physicsProcessor = nullptr;

    return out;
}

char PhysicsProcessorBuilder::parseConfigFiles(){
    if (macroManager->parseFile(macroConfigFilePath) != 0){
        error += macroManager->getError();
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles failed to parse macros file\n";
        return 1;
    }
    float* simWidth = macroManager->getMacro("SIM_WIDTH");
    if (simWidth == nullptr){
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles macro 'SIM_WIDTH' not found\n";
        return 1;
    }
    this->simWidth = *simWidth;

    float* simHeight = macroManager->getMacro("SIM_HEIGHT");
    if (simHeight == nullptr){
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles macro 'SIM_HEIGHT' not found\n";
        return 1;
    }
    this->simHeight = *simHeight;

    structTree->setStructDirectory(structDir);
    if (structTree->setRootStruct(structRootFile, clParser) != 0){
        error += structTree->getError();
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles failed to parse struct root file\n";
        return 2;
    }

    if (kernelQueueBuilder->parseConfig(kernelConfigFilePath) != 0){
        error += kernelCollector->getError();
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles failed to parse kernel config file\n";
        return 3;
    }

    if (substanceCollector->parseConfig(substanceConfigFilePath) != 0){
        error += substanceCollector->getError();
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles failed to parse substance config file\n";
        return 4;
    }

    return 0;
}

void PhysicsProcessorBuilder::createPhysicsProcessor(){
    if (physicsProcessor != nullptr){
        delete physicsProcessor;
    }
    physicsProcessor = new PhysicsProcessor(kernelQueueBuilder->getKernelQueueSize());

    physicsProcessor->globalWorkSize = cl::NDRange(simWidth, simHeight);
    physicsProcessor->countVoxelsSize = cl::NDRange(256);
    physicsProcessor->localWorkSize = localWorkSize;
    physicsProcessor->TBO = TBO;
}


static void CL_CALLBACK clCallback(const char *errInfo, const void *private_info, size_t cb, void *user_data) {
    std::fprintf(stderr ,"RUNTIME ERROR: OpenCL error message: %s\n",errInfo);
}

static void CL_CALLBACK clCustomCallback(const char *errInfo, const void *private_info, size_t cb, void *user_data){
    std::function<void(std::string)>* func = (std::function<void(std::string)>*)user_data;
    (*func)(errInfo);
}

char PhysicsProcessorBuilder::createClContext(){
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);

    if (allPlatforms.empty()){
        error += "ERR: PhysicsProcessorBuilder::createClContext no platforms found\n";
        return 1;
    }

    if (allPlatforms.size() <= clPlatformID){
        error += "ERR: PhysicsProcessorBuilder::createClContext specified platform does not exist (platform 0 will be used)\n";
        clPlatformID = 0;
    }

    cl::Platform defaultPlatform = allPlatforms[clPlatformID];

    std::vector<cl::Device> allDevices;
    defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);


    if (allDevices.size() == 0){
        error += "ERR: PhysicsProcessorBuilder::createClContext no devices found\n";
        return 2;
    }

    if (allDevices.size() <= clDeviceID){
        error += "WARNING: PhysicsProcessorBuilder::createClContext specified device does not exist (device 0 will be used)\n";
        clDeviceID = 0;
    }

    cl::Device device = allDevices[clDeviceID];
    clDeviceName = device.getInfo<CL_DEVICE_NAME>();

    cl_int err;
    cl::Context context;

    if (defaultToFallback){
        physicsProcessor->fallback = true;
        error += "WARNING: PhysicsProcessorBuilder::createClContext set to fallback mode\n";
    } else {
        cl_platform_id platform;
        clGetPlatformIDs(1, &platform, NULL);

#ifdef __APPLE__

        CGLContextObj glContext = CGLGetCurrentContext();
        CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);

        cl_context_properties properties[] = {
            CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
            (cl_context_properties)shareGroup,
            0
        };

#elif __WIN32__

        cl_context_properties properties[] = {
            CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
            CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
            CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
            0
        };

#else

        cl_context_properties properties[] = {
            CL_GL_CONTEXT_KHR, (cl_context_properties) glXGetCurrentContext(),
            CL_GLX_DISPLAY_KHR, (cl_context_properties) glXGetCurrentDisplay(),
            CL_CONTEXT_PLATFORM, (cl_context_properties) platform,
            0
        };

#endif

        physicsProcessor->fallback = false;
        if (clErrorFunction != nullptr){
            context = cl::Context(device, properties, clCustomCallback, clErrorFunction, &err);
        } else {
            context = cl::Context(device, properties, clCallback, nullptr, &err);
        }
        if (err != CL_SUCCESS){
            error += "WARNING: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + " (" + std::to_string(err) + ")\n";
            error += "WARNING: PhysicsProcessorBuilder::createClContext trying fallback mode\n";
            physicsProcessor->fallback = true;
        }
    }

    if (physicsProcessor->fallback){
        if (clErrorFunction != nullptr){
            context = cl::Context(device, NULL, clCustomCallback, clErrorFunction, &err);
        } else {
            context = cl::Context(device, NULL, clCallback, nullptr, &err);
        }
    }

    if (context() == NULL) {
        error += "ERR: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + " (NO ERROR SPECIFIED)\n";
        return 3;
    }

    physicsProcessor->context = context;
    physicsProcessor->device = device;
    physicsProcessor->queue = cl::CommandQueue(context, device);

    cl_uint addressBits;
    device.getInfo(CL_DEVICE_ADDRESS_BITS, &addressBits);
    sizeCalculator = new SizeCalculator(addressBits/8);

    return 0;
}

char PhysicsProcessorBuilder::checkLocalWorkSize(){
    if (localWorkSize[0] == 0 || localWorkSize[1] == 0){
        error += "ERR: PhysicsProcessorBuilder::checkLocalWorkSize local work size was not set\n";
        return 1;
    }

    size_t maxLocalWorkSize;
    clGetDeviceInfo(physicsProcessor->device(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxLocalWorkSize, NULL);

    if (localWorkSize[0] * localWorkSize[1] > maxLocalWorkSize){
        error += "ERR: PhysicsProcessorBuilder::checkLocalWorkSize local work size is too big (" + std::to_string(int(localWorkSize[0])) + " * " + std::to_string(int(localWorkSize[0])) + " > " + std::to_string(maxLocalWorkSize) + ") for device: " + clDeviceName + "\n";
        return 2;
    }

    if (simWidth % localWorkSize[0] != 0 || simHeight % localWorkSize[1] != 0){
        error += "ERR: PhysicsProcessorBuilder::checkLocalWorkSize local work size does not divide simulation size: " + std::to_string(simWidth) + "x" + std::to_string(simHeight) + "\n";
        return 3;
    }

    size_t maxWorkItemSizes[3];
    clGetDeviceInfo(physicsProcessor->device(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(maxWorkItemSizes), &maxWorkItemSizes, NULL);

    if (localWorkSize[0] > maxWorkItemSizes[0] || localWorkSize[1] > maxWorkItemSizes[1]){
        error += "ERR: PhysicsProcessorBuilder::checkLocalWorkSize local work size is too big (" + std::to_string(int(localWorkSize[0])) + " > " + std::to_string(maxWorkItemSizes[0]) + " or " + std::to_string(int(localWorkSize[1])) + " > " + std::to_string(maxWorkItemSizes[1]) + ") for device: " + clDeviceName + "\n";
        return 4;
    }

    if (physicsProcessor->countVoxelsSize[0] > localWorkSize[0] * localWorkSize[1]){
        physicsProcessor->countVoxelsSize = cl::NDRange(localWorkSize[0] * localWorkSize[1]);
    }

    return 0;
}

char PhysicsProcessorBuilder::createSubstanceStructure(){
    std::string subsStructCode = substanceCollector->createSubstanceClStruct();
    std::string subsStructFilePath = structDir + "/substance.cl";
    std::ofstream file(subsStructFilePath);
    if (file.is_open()){
        file << subsStructCode;
        file.close();
    } else {
        error += "ERR: PhysicsProcessorBuilder::createSubstanceStructure failed to write substance structure to file\n";
        return 1;
    }

    return 0;
}

char PhysicsProcessorBuilder::buildStructTree(){
    if (structTree->build(clParser) != 0){
        error += structTree->getError();
        error += "ERR: PhysicsProcessorBuilder::buildStructTree failed to build struct tree\n";
        return 1;
    }

    if (structTree->calculateSizes(sizeCalculator) != 0){
        error += structTree->getError();
        error += "ERR: PhysicsProcessorBuilder::buildStructTree failed to calculate struct sizes\n";
        return 2;
    }

    return 0;
}

char PhysicsProcessorBuilder::addConfigStructure(){
    std::string configStructCode = "";
    std::ifstream file(configStructFile);
    if (file.is_open()){
        std::string line;
        while (std::getline(file, line)){
            configStructCode += line + "\n";
        }
        file.close();
    } else {
        error += "ERR: PhysicsProcessorBuilder::addConfigStructure failed to open config struct file\n";
        return 1;
    }

    configStructure = clParser->processStruct(configStructCode);
    if (configStructure == nullptr){
        error += clParser->getError();
        error += "ERR: PhysicsProcessorBuilder::addConfigStructure failed to process config struct\n";
        return 2;
    }

    if (sizeCalculator->calculate(configStructure) != 0){
        error += sizeCalculator->getError();
        error += "ERR: PhysicsProcessorBuilder::addConfigStructure failed to calculate size of config struct\n";
        return 3;
    }

    return 0;
}

char PhysicsProcessorBuilder::loadKernels(){
    if (kernelQueueBuilder->collectKernels(*kernelCollector) != 0){
        error += kernelQueueBuilder->getError();
        error += "ERR: PhysicsProcessorBuilder::loadKernels failed to collect kernels\n";
        return 1;
    }

    if (addMandatoryKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::loadKernels failed to add mandatory kernels\n";
        return 2;
    }
    return 0;
}

char PhysicsProcessorBuilder::addMandatoryKernels(){
    const std::string spawnVoxelKernelName = "spawn_voxel";
    std::string spawnVoxelKernelCode = "";
    std::ifstream file(mandatoryKernelsDir + spawnVoxelKernelName + ".cl");
    if (file.is_open()){
        std::string line;
        while (std::getline(file, line)){
            spawnVoxelKernelCode += line + "\n";
        }
        file.close();
    } else {
        error += "ERR: PhysicsProcessorBuilder::addMandatoryKernels failed to open file: " + mandatoryKernelsDir + spawnVoxelKernelName + ".cl\n";
        return 1;
    }


    const std::string spawnVoxelsInAreaKernelName = "spawn_voxels_in_area";
    std::string spawnVoxelsInAreaKernelCode;
    std::ifstream file2(mandatoryKernelsDir + spawnVoxelsInAreaKernelName + ".cl");
    if (file2.is_open()){
        std::string line;
        while (std::getline(file2, line)){
            spawnVoxelsInAreaKernelCode += line + "\n";
        }
        file2.close();
    } else {
        error += "ERR: PhysicsProcessorBuilder::addMandatoryKernels failed to open file: " + mandatoryKernelsDir + spawnVoxelsInAreaKernelName + ".cl\n";
        return 2;
    }

    const std::string countVoxelsKernelName = "count_voxels";
    std::string countVoxelsKernelCode;
    std::ifstream file3(mandatoryKernelsDir + countVoxelsKernelName + ".cl");
    if (file3.is_open()){
        std::string line;
        while (std::getline(file3, line)){
            countVoxelsKernelCode += line + "\n";
        }
        file3.close();
    } else {
        error += "ERR: PhysicsProcessorBuilder::addMandatoryKernels failed to open file: " + mandatoryKernelsDir + countVoxelsKernelName + ".cl\n";
        return 3;
    }

    kernelCollector->addKernelCode(spawnVoxelKernelCode, spawnVoxelKernelName);
    kernelCollector->addKernelCode(spawnVoxelsInAreaKernelCode, spawnVoxelsInAreaKernelName);
    kernelCollector->addKernelCode(countVoxelsKernelCode, countVoxelsKernelName);

    return 0;
}

char PhysicsProcessorBuilder::compileCl(){
    cl::Program::Sources sources;
    std::string code = structTree->getStructures();
    code += configStructure->rawCode;
    code += kernelCollector->getKernels();
    sources.push_back({code.c_str(), code.length()});

    program = cl::Program(physicsProcessor->context, sources);

    cl_int buildCode = program.build();

    if (buildCode != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::compileCl failed to compile OpenCL program: " + translateClBuildError(buildCode) + "(" + std::to_string(buildCode) +")\n";
        error += "BUILD LOG:\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(physicsProcessor->device);
        error += "SOURCE CODE:\n" + code;
        return 1;
    }
    return 0;
}

char PhysicsProcessorBuilder::acquireGlObjectFromTBO(){
    if (physicsProcessor->fallback == false){
        cl_int error;

        GLint texWidth, texHeight;
        glBindTexture(GL_TEXTURE_2D, TBO);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
        glBindTexture(GL_TEXTURE_2D, 0);
        if (texWidth != simWidth || texHeight != simHeight){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromTBO TBO size does not match simulation size (" + std::to_string(texWidth) + "x" + std::to_string(texHeight) + " != " + std::to_string(simWidth) + "x" + std::to_string(simHeight) + ")\n";
            return 1;
        }

        physicsProcessor->TBOMemory = clCreateFromGLTexture(physicsProcessor->context(), CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, TBO, &error);

        if (error != CL_SUCCESS){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromTBO failed to create OpenCL memory object from TBO\n";
            return 1;
        }

        physicsProcessor->TBOBuffer = cl::Image2D(physicsProcessor->TBOMemory); 
        clEnqueueAcquireGLObjects(physicsProcessor->queue(), 1, &physicsProcessor->TBOMemory, 0, NULL, NULL);
        physicsProcessor->hostFallbackBuffer = nullptr;

    } else {
        cl_int error;

        GLint texWidth, texHeight;
        glBindTexture(GL_TEXTURE_2D, TBO);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);
        glBindTexture(GL_TEXTURE_2D, 0);
        if (texWidth != simWidth || texHeight != simHeight){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromTBO TBO size does not match simulation size (" + std::to_string(texWidth) + "x" + std::to_string(texHeight) + " != " + std::to_string(simWidth) + "x" + std::to_string(simHeight) + ")\n";
            return 1;
        }

        cl::ImageFormat format(CL_RGBA, CL_FLOAT);
        physicsProcessor->TBOBuffer = cl::Image2D(physicsProcessor->context, CL_MEM_READ_WRITE, format, texWidth, texHeight, 0, nullptr, &error);
        if (error != CL_SUCCESS){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromTBO failed to create OpenCL buffer\n";
            return 1;
        }

        physicsProcessor->hostFallbackBuffer = new float[texWidth*texHeight*4];

#ifdef __APPLE__
        physicsProcessor->fallbackRegion[0] = static_cast<size_t>(texWidth);
        physicsProcessor->fallbackRegion[1] = static_cast<size_t>(texHeight);
        physicsProcessor->fallbackRegion[2] = 1;
#else
        physicsProcessor->fallbackOrigin = {0, 0, 0};
        physicsProcessor->fallbackRegion = {static_cast<size_t>(texWidth), static_cast<size_t>(texHeight), 1};
#endif

        error = physicsProcessor->queue.enqueueReadImage(physicsProcessor->TBOBuffer, CL_TRUE, physicsProcessor->fallbackOrigin, physicsProcessor->fallbackRegion, 0, 0, physicsProcessor->hostFallbackBuffer);
        
        if (error != CL_SUCCESS){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromTBO failed to test copy TBO data to OpenCL buffer with error: " + translateClError(error) + "(" +std::to_string(error) + ")\n";
            return 1;
        }

        glBindTexture(GL_TEXTURE_2D, TBO);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_FLOAT, physicsProcessor->hostFallbackBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    return 0;
}

char PhysicsProcessorBuilder::allocateGPUResourcesMemory(uint& allocatedMemory, const bool& verbose){
    const std::vector<const engineStruct*> structs = structTree->unwindTree();
    //1. creating allocation kernel for each structure that needs it
    //2. compilating those kernels and saving them into a map
    //3. invoking recursive allocation starting from the root

    std::map<std::string, cl::Kernel> allocationKernels;
    std::string code = "";
    cl::Program::Sources sources;

    code = structTree->getStructures();

    for (const engineStruct* structure : structs){
        if (allocationKernels.find(structure->name) == allocationKernels.end()){
            code += createAllocationKernel(structure);
            allocationKernels.insert({structure->name, cl::Kernel()});
        }
    }
    sources.push_back({code.c_str(), code.length()});

    cl::Program program = cl::Program(physicsProcessor->context, sources);
    cl_int buildCode = program.build();
    if (buildCode != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::allocateGPUResourcesMemory failed to compile allocation kernels: " + translateClBuildError(buildCode) + "(" + std::to_string(buildCode) +")\n";
        error += "BUILD LOG:\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(physicsProcessor->device);
        error += "SOURCE CODE:\n" + code;
        return 1;
    }

    for (const engineStruct* structure : structs){
        cl::Kernel kernel = cl::Kernel(program, ("set_" + structure->name).c_str());
        if (kernel() == NULL){
            error += "ERR: PhysicsProcessorBuilder::allocateGPUResourcesMemory failed to create allocation kernel for structure: " + structure->name + "\n";
            return 2;
        }
        allocationKernels[structure->name] = kernel;
    }

    if (allocateStructure(structs[0], allocationKernels, physicsProcessor->engineResources, allocatedMemory, 1, verbose) != 0){
        error += "ERR: PhysicsProcessorBuilder::allocateGPUResourcesMemory failed to build structure tree\n";
        return 3;
    }

    return 0;
}

std::string PhysicsProcessorBuilder::createAllocationKernel(const engineStruct* structure){
    std::string kernelCode = "void kernel set_" + structure->name + "(global struct " + structure->name + "* structure, uint index";
    for (uint i = 0; i < structure->fieldCount; i++){
        const engineStruct::field& field = structure->fields[i];
        if (field.arrSize > 0){
            if (field.subStruct == nullptr){
                kernelCode += ", global " + SubstanceCollector::clTypeToString(field.type) + "* " + field.name;
            } else {
                kernelCode += ", global struct " + field.subStruct->name + "* " + field.name;
            }
        }
    }
    kernelCode += "){\n";
    for (uint i = 0; i < structure->fieldCount; i++){
        const engineStruct::field& field = structure->fields[i];
        if (field.arrSize > 0){
            kernelCode += "    structure[index]." + field.name + " = " + field.name + ";\n";
        }
    }
    if (structure->name == "substance"){
        kernelCode += "   printf(\"%f, %f, %f \\n\", structure[index].color.R, structure[index].color.G, structure[index].color.B);";
    }

    kernelCode += "}\n";

    return kernelCode;
}


char PhysicsProcessorBuilder::allocateStructure(const engineStruct* structure, const std::map<std::string, cl::Kernel>& kernels, cl::Buffer*& buffer, uint& allocatedMemory, uint count, const bool& verbose){
    uint toAllocate = structure->byteSize*count;
    if (buffer == nullptr){
        // if (verbose) std::printf("Allocating %s x %u (%dB)\n", structure->name.c_str(), count, structure->byteSize*count);
        buffer = new cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, toAllocate);
        if ((*buffer)() == NULL){
            error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to allocate memory (" + std::to_string(toAllocate) + "B) for structure: " + structure->name + "\n";
            return -1;
        }
    }
    size_t size;
    buffer->getInfo(CL_MEM_SIZE, &size);
    if (verbose)std::printf("Allocated %s (%zuB out of %zuB)\n", structure->name.c_str(), size, toAllocate);
    allocatedMemory += size;

    if (hasPointers(structure) == false){
        return 0;
    }

    for (uint i = 0; i < count; i++){
        cl::Kernel kernel = kernels.at(structure->name);
        kernel.setArg(0, *buffer);
        kernel.setArg(1, i);

        for (uint j = 0; j < structure->fieldCount; j++){
            const engineStruct::field& field = structure->fields[j];
            cl::Buffer* childBuffer = nullptr;
            if (field.arrSize > 0){
                if (field.name == "SUBSTANCES"){
                    if (setSubstancesProperties(childBuffer, allocatedMemory) != 0){
                        error  += "ERR: PhysicsProcessorBuilder::allocateStructure failed to set substances properties\n";
                        return -1;
                    }
                    cl::Kernel kernel2 = kernels.at("substance");
                    kernel2.setArg(0, *childBuffer);

                    for (uint k = 0; k < substanceCollector->getSubstances().size(); k++){
                        kernel2.setArg(1, k);
                        physicsProcessor->queue.enqueueNDRangeKernel(kernel2, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
                        physicsProcessor->queue.finish();
                    }

                    physicsProcessor->allocatedGPUMemory.push_back(childBuffer);
                } else if (field.subStruct == nullptr){
                    uint toAllocate = sizeCalculator->clTypeSize(field.type) * field.arrSize;
                    childBuffer = new cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, toAllocate);
                    if ((*childBuffer)() == NULL){
                        size_t size;
                        childBuffer->getInfo(CL_MEM_SIZE, &size);
                        error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to allocate memory (" + std::to_string(sizeCalculator->clTypeSize(field.type) * field.arrSize) + "B) for field: " + field.name + "\n";
                        return -1;
                    }
                    if (verbose)std::printf("Allocated field %s (%dB out of %dB)\n", field.name.c_str(), size, toAllocate);
                    allocatedMemory += toAllocate;
                    physicsProcessor->allocatedGPUMemory.push_back(childBuffer);
                } else {
                    if (allocateStructure(field.subStruct, kernels, childBuffer, allocatedMemory, field.arrSize, verbose) != 0){
                        return -1;
                    }
                    physicsProcessor->allocatedGPUMemory.push_back(childBuffer);
                }
            }
            kernel.setArg(j+2, *childBuffer);
        }
        cl_int err;
        err = physicsProcessor->queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
        if (err != CL_SUCCESS){
            error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to enqueue kernel for structure: " + structure->name + " " + translateClError(err) + "(" + std::to_string(err) + ")\n";
            return -1;
        }
        err = physicsProcessor->queue.finish();
        if (err != CL_SUCCESS){
            error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to finish queue for structure: " + structure->name + " " + translateClError(err) + "(" + std::to_string(err) + ")\n";
            return -1;
        }
    }

    return 0;
}

bool PhysicsProcessorBuilder::hasPointers(const engineStruct* structure){
    for (uint i = 0; i < structure->fieldCount; i++){
        if (structure->fields[i].arrSize > 0){
            return true;
        }
        if (structure->fields[i].subStruct != nullptr){
            if (hasPointers(structure->fields[i].subStruct)){
                return true;
            }
        }
    }
    return false;
}

char PhysicsProcessorBuilder::setSubstancesProperties(cl::Buffer*& buffer, uint& allocatedMemory){
    const std::vector<substance>& substances = substanceCollector->getSubstances();
    const std::vector<substanceField>& properties = substanceCollector->getSubstancePhroperties();

    const engineStruct* subsStruct;
    for (const engineStruct* structure : structTree->unwindTree()){
        if (structure->name == "substance"){
            subsStruct = structure;
            break;
        }
    }
    uint toAllocate = subsStruct->byteSize * substances.size();
    std::printf("Struct size: %u, count: %u, total: %u\n", subsStruct->byteSize, substances.size(), toAllocate);
    buffer = new cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, toAllocate);
    if ((*buffer)() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setSubstancesProperties failed to allocate memory for substances\n";
        return 1;
    }
    allocatedMemory += toAllocate;

    std::string code = structTree->getStructures();
    code += "void kernel set_substances(global struct substance* substances, uint index";
    code += ", uint movable, float R, float G, float B";
    for (uint i = 0; i < properties.size(); i++){
        code += ", float " + properties[i].name;
    }
    code += "){\n";
    code += "    substances[index].movable = movable;\n";
    code += "    substances[index].color.R = R;\n";
    code += "    substances[index].color.G = G;\n";
    code += "    substances[index].color.B = B;\n";
    code += "    substances[index].color.A = 1.0f;\n";
    for (uint i = 0; i < properties.size(); i++){
        code += "    substances[index]." + properties[i].name + " = " + properties[i].name + ";\n";
    }
    code += "}\n";

    cl::Program::Sources sources;
    sources.push_back({code.c_str(), code.length()});

    cl::Program program = cl::Program(physicsProcessor->context, sources);

    cl_int buildCode = program.build();
    if (buildCode != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::setSubstancesProperties failed to compile set_substances kernel: " + translateClBuildError(buildCode) + "(" + std::to_string(buildCode) +")\n";
        error += "BUILD LOG:\n" + program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(physicsProcessor->device);
        error += "SOURCE CODE:\n" + code;
        return 2;
    }

    cl::Kernel kernel = cl::Kernel(program, "set_substances");

    if (kernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setSubstancesProperties failed to create set_substances kernel\n";
        return 3;
    }

    // cl_uint maxParameterSize = physicsProcessor->device.getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>();
    // std::printf("Max parameter size: %u\n", maxParameterSize);

    kernel.setArg(0, *buffer);
    for (uint i = 0; i < substances.size(); i++){
        kernel.setArg(1, i);
        kernel.setArg(2, uint(substances[i].movable));
        kernel.setArg(3, substances[i].R);
        kernel.setArg(4, substances[i].G);
        kernel.setArg(5, substances[i].B);
        for (uint j = 0; j < properties.size(); j++){
            kernel.setArg(j+6, substances[i].values[j]);
        }
        cl_int err;
        err = physicsProcessor->queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
        if (err != CL_SUCCESS){
            error += "ERR: PhysicsProcessorBuilder::setSubstancesProperties failed to enqueue kernel for substance: " + substances[i].name + " " + translateClError(err) + "(" + std::to_string(err) + ")\n";
            return 3;
        }
        err = physicsProcessor->queue.finish();
        if (err != CL_SUCCESS){
            error += "ERR: PhysicsProcessorBuilder::setSubstancesProperties failed to finish queue for substance: " + substances[i].name + " " + translateClError(err) + "(" + std::to_string(err) + ")\n";
            return 4;
        }
    }


    return 0;
}

char PhysicsProcessorBuilder::allocateGPUConfigStructure(){
    physicsProcessor->engineConfig = cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, configStructure->byteSize);

    if (physicsProcessor->engineConfig() == NULL){
        error += "ERR: PhysicsProcessorBuilder::allocateGPUConfigStructure failed to allocate memory for config structure\n";
        return 1;
    }

    std::string code = configStructure->rawCode;
    code += createConfigStructureKernel();

    cl::Program::Sources sources;
    sources.push_back({code.c_str(), code.length()});

    cl::Program program = cl::Program(physicsProcessor->context, sources);
    cl_int buildCode = program.build();
    if (buildCode != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::allocateGPUConfigStructure failed to compile config structure kernel\n";
        return 2;
    }

    cl::Kernel kernel = cl::Kernel(program, "set_config_structure");
    if (kernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::allocateGPUConfigStructure failed to create config structure kernel\n";
        return 3;
    }

    kernel.setArg(0, physicsProcessor->engineConfig);

    physicsProcessor->queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));

    return 0;
}

std::string PhysicsProcessorBuilder::createConfigStructureKernel(){
    std::string kernelCode = "void kernel set_config_structure(global struct engineConfig* config){\n";
    for (uint i = 0; i < configStructure->fieldCount; i++){
        const engineStruct::field& field = configStructure->fields[i];
        kernelCode += "    config->" + field.name + " = " + std::to_string(field.defaultValue) + ";\n";
    }
    kernelCode += "}\n";
    return kernelCode;
}


char PhysicsProcessorBuilder::allocateGPUWorkBuffers(){
    physicsProcessor->countVoxelReturnValue = cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, sizeof(uint));
    physicsProcessor->countVoxelWorkMemory = cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, sizeof(cl_uint) * simHeight * simWidth / 2 + ((simHeight * simWidth) & 0x1));
    return 0;
}

char PhysicsProcessorBuilder::setMandatoryKernels(){
    physicsProcessor->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
    if (physicsProcessor->spawn_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxel kernel\n";
        return 1;
    }
    physicsProcessor->spawn_voxelKernel.setArg(3, *physicsProcessor->engineResources);
    physicsProcessor->spawn_voxelKernel.setArg(4, physicsProcessor->engineConfig);

    physicsProcessor->spawn_voxel_in_areaKernel = cl::Kernel(program, "spawn_voxels_in_area");
    if (physicsProcessor->spawn_voxel_in_areaKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxels_in_area kernel\n";
        return 2;
    }
    physicsProcessor->spawn_voxel_in_areaKernel.setArg(3, *physicsProcessor->engineResources);
    physicsProcessor->spawn_voxel_in_areaKernel.setArg(4, physicsProcessor->engineConfig);

    physicsProcessor->count_voxelKernel = cl::Kernel(program, "count_voxels");
    if (physicsProcessor->count_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create count_voxels kernel\n";
        return 3;
    }
    physicsProcessor->count_voxelKernel.setArg(0, *physicsProcessor->engineResources);
    physicsProcessor->count_voxelKernel.setArg(1, physicsProcessor->countVoxelWorkMemory);
    physicsProcessor->count_voxelKernel.setArg(2, simHeight * simHeight);
    physicsProcessor->count_voxelKernel.setArg(3, physicsProcessor->countVoxelReturnValue);

    return 0;
}

char PhysicsProcessorBuilder::setKernelQueue(){
    const std::vector<kernelExecutionUnit>& kernelQueue = kernelQueueBuilder->getKernelQueue();

    uint engineIterator = 0;


    for (uint i = 0; i < kernelQueue.size(); i++){
        const kernelExecutionUnit& keu = kernelQueue[i];
        cl::Kernel kernel(program, keu.functionName.c_str());
        if (kernel() == NULL){
            error += "ERR: PhysicsProcessorBuilder::setKernelQueue failed to create engine kernel named: "  + keu.functionName + "\n";
            return 1;
        }
        kernel.setArg(0, physicsProcessor->engineConfig);
        kernel.setArg(1, *physicsProcessor->engineResources);
        kernel.setArg(2, physicsProcessor->TBOBuffer);
        
        for (uint j = 0; j < keu.executionCount; j++){
            physicsProcessor->engine[engineIterator] = kernel;
            engineIterator++;
        }
    }

    return 0;
}
