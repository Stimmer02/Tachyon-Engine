#include "PhysicsProcessorBuilder.h"

PhysicsProcessorBuilder::PhysicsProcessorBuilder(){
    physicsProcessor = nullptr;

    structTree = new StructTree();
    sizeCalculator = new SizeCalculator(8);
    clParser = new ClStructParser(macroManager);
    macroManager = new MacroManager();
    kernelCollector = new KernelCollector();
    kernelQueueBuilder = new KernelQueueBuilder();
    substanceCollector = new SubstanceCollector();

    error = "";
    kernelConfigFilePath = "";
    macroConfigFilePath = "";
    substanceConfigFilePath = "";
    structDir = "";
    structRootFile = "";
    clPlatformID = 0;
    clDeviceID = 0;
}

PhysicsProcessorBuilder::~PhysicsProcessorBuilder(){
    delete structTree;
    delete sizeCalculator;
    delete clParser;
    delete macroManager;
    delete kernelCollector;
    delete kernelQueueBuilder;
    delete substanceCollector;
}


char PhysicsProcessorBuilder::parseSystemConfig(std::string path){

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


void PhysicsProcessorBuilder::setClPlatform(cl_uint platform){
    clPlatformID = platform;
}

void PhysicsProcessorBuilder::setClDevice(cl_uint device){
    clDeviceID = device;
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

char PhysicsProcessorBuilder::build(){
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

    if (parseConfigFiles() != 0){ 
        error += "ERR: PhysicsProcessorBuilder::build failed to parse config files\n";
        return 6;
    }
    
    if (loadKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to load kernels\n";
        return 7;
    }

    physicsProcessor = new PhysicsProcessor(kernelQueueBuilder->getKernelQueueSize());

    if (createClContext() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create cl context\n";
        return 8;
    }

    if (createSubstanceStructure() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to create substance structure\n";
        return 9;
    }

    if (buildStructTree() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to build struct tree\n";
        return 10;
    }

    if (compileCl() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to compile OpenCL program\n";
        return 11;
    }

    if (setMandatoryKernels() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set mandatory kernels\n";
        return 12;
    }

    if (setKernelQueue() != 0){
        error += "ERR: PhysicsProcessorBuilder::build failed to set kernel queue\n";
        return 13;
    }

    return 0;
}

char PhysicsProcessorBuilder::parseConfigFiles(){
    if (macroManager->parseFile(macroConfigFilePath) != 0){
        error += macroManager->getError();
        error += "ERR: PhysicsProcessorBuilder::parseConfigFiles failed to parse macros file\n";
        return 1;
    }

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

    cl::Device defaultDevice = allDevices[clDeviceID];
    clDeviceName = defaultDevice.getInfo<CL_DEVICE_NAME>();

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
    cl::Context context(defaultDevice, properties);
    if (context() == NULL) {
        error += "ERR: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + "\n";
        return 3;
    }

    physicsProcessor->context = context;

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
        "   void kernel spawn_voxel_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){"
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

    kernelCollector->addKernelCode(spawnVoxelKernelName, spawnVoxelKernelCode);
    kernelCollector->addKernelCode(spawnVoxelsInAreaKernelName, spawnVoxelsInAreaKernelCode);
    kernelCollector->addKernelCode(countVoxelsKernelName, countVoxelsKernelCode);
}

char PhysicsProcessorBuilder::compileCl(){
    cl::Program::Sources sources;
    std::string code = structTree->getStructures();
    code += kernelCollector->getKernels();
    sources.push_back({code.c_str(), code.length()});

    program = cl::Program(physicsProcessor->context, sources);

    cl_int buildCode = program.build();

    if (buildCode != CL_SUCCESS) {
        error += "ERR: PhysicsProcessorBuilder::compileCl failed to compile OpenCL program\n";
        return 1;
        // std::fprintf(stderr ,"Error building TACHYON_ENGINE code: %d\n", buildCode);
        // std::fprintf(stderr ,"Trying fallback settings...\n");

        // context = cl::Context(defaultDevice);
        // program = cl::Program(context, sources);

        // buildCode = program.build();
        // if (buildCode != CL_SUCCESS) {
        //     std::fprintf(stderr ,"Error building TACHYON_ENGINE %d: %s\n", buildCode, PhysicsProcessorBuilder::getErrorString(buildCode).c_str());
        //     std::fprintf(stderr ,"Error building TACHYON_ENGINE content:\n%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice).c_str());
        //     std::fprintf(stderr ,"ERROR: not able to compile TACHYON_ENGINE kernel!\n");
        //     return nullptr;
        // }
        // std::fprintf(stderr ,"WARNING: entering fallback mode\n");
        // std::printf("Compillation TACHYON_ENGINE successful!\n");
        // cl::Kernel TACHYON_ENGINE(program, "TACHYON_ENGINE");
        // return PhysicsProcesor = new PhysicsProcessor_Fallback(context, TACHYON_ENGINE, PBO, config, defaultDevice);
    }

    return 0;
}

char PhysicsProcessorBuilder::setMandatoryKernels(){
    physicsProcessor->spawn_voxelKernel = cl::Kernel(program, "spawn_voxel");
    if (physicsProcessor->spawn_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxel kernel\n";
        return 1;
    }

    physicsProcessor->spawn_voxel_in_areaKernel = cl::Kernel(program, "spawn_voxels_in_area");
    if (physicsProcessor->spawn_voxel_in_areaKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create spawn_voxels_in_area kernel\n";
        return 2;
    }

    physicsProcessor->count_voxelKernel = cl::Kernel(program, "count_voxels");
    if (physicsProcessor->count_voxelKernel() == NULL){
        error += "ERR: PhysicsProcessorBuilder::setMandatoryKernels failed to create count_voxels kernel\n";
        return 3;
    }

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
        for (uint j = 0; j < keu.executionCount; j++){
            physicsProcessor->engine[engineIterator] = kernel;
            engineIterator++;
        }
    }
    
    return 0;
}