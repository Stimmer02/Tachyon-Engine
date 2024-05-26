#include "PhysicsProcessorBuilder.h"

PhysicsProcessorBuilder::PhysicsProcessorBuilder(){
    physicsProcessor = nullptr;

    structTree = new StructTree();
    sizeCalculator = new SizeCalculator(8);
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

    clDeviceName = "";

    localWorkSize = cl::NDRange(0);

    PBO = 0;
}

PhysicsProcessorBuilder::~PhysicsProcessorBuilder(){
    delete structTree;
    delete sizeCalculator;
    delete clParser;
    delete macroManager;
    delete kernelCollector;
    delete kernelQueueBuilder;
    delete substanceCollector;

    if (physicsProcessor != nullptr){
        delete physicsProcessor;
    }

    if (configStructure != nullptr){
        delete configStructure;
    }
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

    int platform;
    int device;

    config.ParseInt("CL_PLATFORM_ID", platform, -1);
    config.ParseInt("CL_DEVICE_ID", device, -1);
    if (platform == -1 || device == -1){
        error += "ERR: PhysicsProcessorBuilder::parseSystemConfig CL_PLATFORM_ID or CL_DEVICE_ID not found\n";
        return 7;
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

void PhysicsProcessorBuilder::setClPlatformAndDevice(cl_uint platform, cl_uint device){
    clPlatformID = platform;
    clDeviceID = device;
}

void PhysicsProcessorBuilder::setLocalWorkSize(cl::NDRange localWorkSize){
    this->localWorkSize = localWorkSize;
}


void PhysicsProcessorBuilder::setClPlatform(cl_uint platform){
    clPlatformID = platform;
}

void PhysicsProcessorBuilder::setClDevice(cl_uint device){
    clDeviceID = device;
}

void PhysicsProcessorBuilder::setPBO(GLuint PBO){
    this->PBO = PBO;
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
        case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case CL_PLATFORM_NOT_FOUND_KHR: return "CL_PLATFORM_NOT_FOUND_KHR";
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
    if (PBO == 0){
        error += "ERR: PhysicsProcessorBuilder::build PBO not set\n";
        return 7;
    }

    if (verbose)std::printf("Parsing config files\n");
    if (parseConfigFiles() != 0){ 
        error += "ERR: PhysicsProcessorBuilder::build failed to parse config files\n";
        return 8;
    }
    
    if (verbose)std::printf("Loading kernels\n");
    if (loadKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to load kernels\n";
        return 9;
    }

    if (verbose)std::printf("Creating physics processor\n");
    createPhysicsProcessor();

    if (verbose)std::printf("Creating OpenCL context\n");
    if (createClContext() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create cl context\n";
        return 10;
    }

    if (verbose)std::printf("Checking local work size\n");
    if (checkLocalWorkSize() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed during local work size check\n";
        return 11;
    }

    if (verbose)std::printf("Creating substance structure\n");
    if (createSubstanceStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create substance structure\n";
        return 12;
    }

    if (verbose)std::printf("Building struct tree\n");
    if (buildStructTree() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to build struct tree\n";
        return 13;
    }

    if (verbose)std::printf("Adding config structure\n");
    if (addConfigStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to add config structure\n";
        return 14;
    }

    if (verbose)std::printf("Compiling OpenCL program\n");
    if (compileCl() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to compile OpenCL program\n";
        return 15;
    }

    if (verbose)std::printf("Setting mandatory kernels\n");
    if (setMandatoryKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set mandatory kernels\n";
        return 16;
    }

    if (verbose)std::printf("Setting kernel queue (engine)\n");
    if (setKernelQueue() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set kernel queue\n";
        return 17;
    }

    // if (verbose)std::printf("Acquiring GL object from PBO\n"
    // if (acquireGlObjectFromPBO() != 0){
    //     error += "ERR: PhysicsProcessorBuilder::build failed to acquire GL object from PBO\n";
    //     return 18;
    // }

    if (verbose)std::printf("Allocating GPU resources memory\n");
    if (allocateGPUResourcesMemory() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to allocate GPU memory\n";
        return 19;
    }

    if (verbose)std::printf("Allocating GPU config structure\n");
    if (allocateGPUConfigStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to allocate GPU memory for config structure\n";
        return 20;
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
    physicsProcessor->localWorkSize = localWorkSize;
}

void PhysicsProcessorBuilder::appendError(const std::string& err) {
    error += err;
}

static void CL_CALLBACK contextCallback(const char *errInfo, const void *private_info, size_t cb, void *user_data) {
    PhysicsProcessorBuilder* self = static_cast<PhysicsProcessorBuilder*>(user_data);
    self->appendError("ERR: PhysicsProcessorBuilder: OpenCL error message: " + std::string(errInfo));
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
        error += "ERR: PhysicsProcessorBuilder::createClContext specified device does not exist (device 0 will be used)\n";
        clDeviceID = 0;
    }

    cl::Device device = allDevices[clDeviceID];
    clDeviceName = device.getInfo<CL_DEVICE_NAME>();

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
    cl_int err;
    cl::Context context(device, properties, contextCallback, this, &err);
    if (err != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + " (" + std::to_string(err) + ")\n";
        error += "WARNING: PhysicsProcessorBuilder::createClContext trying fallback mode\n";
        physicsProcessor->fallback = true;
        context = cl::Context(device, NULL, contextCallback, this, &err);
    }
    
    if (context() == NULL) {
        error += "ERR: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + " (NO ERROR SPECIFIED)\n";
        return 3;
    }

    physicsProcessor->context = context;
    physicsProcessor->device = device;
    physicsProcessor->queue = cl::CommandQueue(context, device);

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

    addMandatoryKernels();
    return 0;
}

void PhysicsProcessorBuilder::addMandatoryKernels(){
    const std::string spawnVoxelKernelName = "spawn_voxel";
    const std::string spawnVoxelKernelCode = 
        "    void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.x = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.y = 0;"
        "        resources->worldMap->voxels[y * config->simulationWidth + x].substanceID = substanceID;"
        "    }";

    const std::string spawnVoxelsInAreaKernelName = "spawn_voxels_in_area";
    const std::string spawnVoxelsInAreaKernelCode = 
        "   void kernel spawn_voxels_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
        "       uint globalID, IDX, IDY;"
        "       IDX = x + get_global_id(0);"
        "       IDY = y + get_global_id(1);"
        "       globalID = IDY * config->simulationWidth + IDX;"
        ""
        "        if (config->simulationWidth > IDX && config->simulationHeight > IDY){"
        "           resources->worldMap->voxels[globalID].forceVector.x = 0;"
        "           resources->worldMap->voxels[globalID].forceVector.y = 0;"
        "           resources->worldMap->voxels[globalID].substanceID = substanceID;"
        "        }"
        "   }";

    const std::string countVoxelsKernelName = "count_voxels";
    const std::string countVoxelsKernelCode = 
        "    void kernel count_voxels(global struct engineResources* resources, global uint* workArr, uint size, global uint* returnValue){"
        "        private uint id = get_global_id(0);"
        "        private uint dim = get_local_size(0);"
        "        private char dividionRest = size & 0x1;"
        "        private uint currentSize = size >> 1;"
        "        private uint index;"
        ""
        "        for (private uint i = id; i < currentSize; i += dim){"
        "           index = i << 1;"
        "            workArr[i] = (resources->worldMap->voxels[index].substanceID > 0) + (resources->worldMap->voxels[index+1].substanceID > 0);"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        if (dividionRest){"
        "            if (id == 0){"
        "               workArr[currentSize] = (resources->worldMap->voxels[currentSize << 1].substanceID > 0);"
        "           }"
        "           currentSize++;"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        dividionRest = currentSize & 0x1;"
        "        currentSize >>= 1;"
        ""
        "        while (currentSize > 1){"
        "            for (private uint i = id; i < currentSize; i += dim){"
        "               index = i << 1;"
        "               workArr[i] = workArr[index] + workArr[index+1];"
        "            }"
        "            barrier(CLK_LOCAL_MEM_FENCE);"
        "            if (dividionRest){"
        "               if (id == 0){"
        "                   workArr[currentSize] = workArr[currentSize << 1];"
        "               }"
        "               currentSize++;"
        "           }"
        "            barrier(CLK_LOCAL_MEM_FENCE);"
        "            dividionRest = currentSize & 0x1;"
        "            currentSize >>= 1;"
        "        }"
        "        barrier(CLK_LOCAL_MEM_FENCE);"
        "        if (id == 0){"
        "            *returnValue = workArr[0] + workArr[1];"
        "            if (dividionRest == 1){"
        "               *returnValue += workArr[2];"
        "            }"
        "       }"
        "   }";

    kernelCollector->addKernelCode(spawnVoxelKernelCode, spawnVoxelKernelName);
    kernelCollector->addKernelCode(spawnVoxelsInAreaKernelCode, spawnVoxelsInAreaKernelName);
    kernelCollector->addKernelCode(countVoxelsKernelCode, countVoxelsKernelName);
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

char PhysicsProcessorBuilder::setMandatoryKernels(){
    physicsProcessor->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
    if (physicsProcessor->spawn_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxel kernel\n";
        return 1;
    }
    physicsProcessor->spawn_voxelKernel.setArg(3, physicsProcessor->engineResources);
    physicsProcessor->spawn_voxelKernel.setArg(4, physicsProcessor->engineConfig);

    physicsProcessor->spawn_voxel_in_areaKernel = cl::Kernel(program, "spawn_voxels_in_area");
    if (physicsProcessor->spawn_voxel_in_areaKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxels_in_area kernel\n";
        return 2;
    }
    physicsProcessor->spawn_voxel_in_areaKernel.setArg(3, physicsProcessor->engineResources);
    physicsProcessor->spawn_voxel_in_areaKernel.setArg(4, physicsProcessor->engineConfig);

    physicsProcessor->count_voxelKernel = cl::Kernel(program, "count_voxels");
    if (physicsProcessor->count_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create count_voxels kernel\n";
        return 3;
    }
    physicsProcessor->count_voxelKernel.setArg(0, physicsProcessor->engineResources);
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
        kernel.setArg(1, physicsProcessor->engineResources);
        for (uint j = 0; j < keu.executionCount; j++){
            physicsProcessor->engine[engineIterator] = kernel;
            engineIterator++;
        }
    }
    
    return 0;
}

char PhysicsProcessorBuilder::allocateGPUResourcesMemory(){
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

    if (allocateStructure(structs[0], allocationKernels, physicsProcessor->engineResources) != 0){
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
    kernelCode += "}\n";

    return kernelCode;
}

char PhysicsProcessorBuilder::allocateStructure(const engineStruct* structure, const std::map<std::string, cl::Kernel>& kernels, cl::Buffer& buffer, uint count){
    buffer = cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, structure->byteSize*count);
    if (buffer() == NULL){
        error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to allocate memory (" + std::to_string(structure->byteSize*count) + "B) for structure: " + structure->name + "\n";
        return -1;
    }

    for (uint i = 0; i < count; i++){
        cl::Kernel kernel = kernels.at(structure->name);
        kernel.setArg(0, buffer);
        kernel.setArg(1, i);

        for (uint j = 0; j < structure->fieldCount; j++){
            const engineStruct::field& field = structure->fields[j];
            cl::Buffer* childBuffer;
            if (field.arrSize > 0){
                if (field.name == "PBO" || field.name == "pbo"){
                    childBuffer = &physicsProcessor->pboBuffer;
                } else if (field.subStruct == nullptr){
                    childBuffer = new cl::Buffer(physicsProcessor->context, CL_MEM_READ_WRITE, sizeCalculator->clTypeSize(field.type) * field.arrSize);
                    if ((*childBuffer)() == NULL){
                        error += "ERR: PhysicsProcessorBuilder::allocateStructure failed to allocate memory (" + std::to_string(sizeCalculator->clTypeSize(field.type) * field.arrSize) + "B) for field: " + field.name + "\n";
                        return -1;
                    }
                    physicsProcessor->allocatedGPUMemory.push_back(childBuffer);
                } else {
                    childBuffer = new cl::Buffer();
                    if (allocateStructure(field.subStruct, kernels, *childBuffer, field.arrSize) != 0){
                        return -1;
                    }
                    physicsProcessor->allocatedGPUMemory.push_back(childBuffer);
                }
            }
            kernel.setArg(j+2, *childBuffer);
        }
        physicsProcessor->queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
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

char PhysicsProcessorBuilder::acquireGlObjectFromPBO(){
    if (physicsProcessor->fallback == false){
        cl_int error;
        physicsProcessor->pboMemory = clCreateFromGLBuffer(physicsProcessor->context(), CL_MEM_WRITE_ONLY, PBO, &error);

        if (error != CL_SUCCESS){
            this->error += "ERR: PhysicsProcessorBuilder::acquireGlObjectFromPBO failed to create OpenCL memory object from PBO\n";
            return 1;
        }

        physicsProcessor->pboBuffer = cl::Buffer(physicsProcessor->pboMemory);
        clEnqueueAcquireGLObjects(physicsProcessor->queue(), 1, &physicsProcessor->pboMemory, 0, NULL, NULL);

        physicsProcessor->hostFallbackBuffer = nullptr;
    } else {
        physicsProcessor->hostFallbackBuffer = new unsigned char[4 * simWidth * simHeight];//!!!!!!! 4 is hardcoded
        physicsProcessor->pboMemory = clCreateBuffer(physicsProcessor->context(), CL_MEM_WRITE_ONLY, 4 * simWidth * simHeight, NULL, NULL);
        physicsProcessor->pboBuffer = cl::Buffer(physicsProcessor->pboMemory);

        glBindBuffer(GL_ARRAY_BUFFER, PBO);
    }
    return 0;
}