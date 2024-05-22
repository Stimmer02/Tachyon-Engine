#include "PhysicsProcessorBuilder.h"

IPhysicsProcessor* PhysicsProcessorBuilder::build(KernelBuilder& kernelBuilder, StructCollector& structCollector, GLuint PBO, engineConfig config, uint platformID, uint deviceID){
    IPhysicsProcessor* PhysicsProcesor;

    cl::Program::Sources sources = structCollector.get();
    std::string buildMessage = kernelBuilder.build(sources);
    std::printf("%s\n", buildMessage.c_str());


    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);

    if (allPlatforms.empty()){
        std::fprintf(stderr, "ERROR: No platforms found. Check OpenCL installation!\n");
        return nullptr;
    }

    if (allPlatforms.size() <= platformID){
        std::fprintf(stderr, "WARNING: Specified platform does not exist\n");
        platformID = 0;
    }

    cl::Platform defaultPlatform = allPlatforms[platformID];
    std::printf("Using platform:\t%s\n", defaultPlatform.getInfo<CL_PLATFORM_NAME>().c_str());

    std::vector<cl::Device> allDevices;
    defaultPlatform.getDevices(CL_DEVICE_TYPE_ALL, &allDevices);


    if (allDevices.size() == 0){
        std::fprintf(stderr, "ERROR: No devices found. Check OpenCL installation!\n");
        return nullptr;
    }

    if (allDevices.size() <= deviceID){
        std::fprintf(stderr, "WARNING: Specified device does not exist\n");
        deviceID = 0;
    }

    cl::Device defaultDevice = allDevices[deviceID];
    std::printf("Using device:\t%s\n", defaultDevice.getInfo<CL_DEVICE_NAME>().c_str());

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

    cl::Program program(context, sources);

    size_t maxLocalWorkSize;
    clGetDeviceInfo(defaultDevice(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxLocalWorkSize, NULL);
    std::printf("max local work size: %ld\n\n", maxLocalWorkSize);

    cl_int buildCode = program.build();

    if (buildCode != CL_SUCCESS) {
        std::fprintf(stderr ,"Error building TACHYON_ENGINE code: %d\n", buildCode);
        std::fprintf(stderr ,"Trying fallback settings...\n");

        context = cl::Context(defaultDevice);
        program = cl::Program(context, sources);

        buildCode = program.build();
        if (buildCode != CL_SUCCESS) {
            std::fprintf(stderr ,"Error building TACHYON_ENGINE %d: %s\n", buildCode, PhysicsProcessorBuilder::getErrorString(buildCode).c_str());
            std::fprintf(stderr ,"Error building TACHYON_ENGINE content:\n%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice).c_str());
            std::fprintf(stderr ,"ERROR: not able to compile TACHYON_ENGINE kernel!\n");
            return nullptr;
        }
        std::fprintf(stderr ,"WARNING: entering fallback mode\n");
        std::printf("Compillation TACHYON_ENGINE successful!\n");
        cl::Kernel TACHYON_ENGINE(program, "TACHYON_ENGINE");
        return PhysicsProcesor = new PhysicsProcessor_Fallback(context, TACHYON_ENGINE, PBO, config, defaultDevice);
    }


    cl::Kernel TACHYON_ENGINE(program, "TACHYON_ENGINE");
    return PhysicsProcesor = new PhysicsProcessor(context, TACHYON_ENGINE, PBO, config, defaultDevice);
}

IPhysicsProcessor* PhysicsProcessorBuilder::build(const std::string kernelFragmentsDirectory, const std::string structsDirectory, GLuint PBO, engineConfig config, uint platformID, uint deviceID){

    StructCollector structCollector;
    if (structsDirectory.empty() == false){
        if (structCollector.loadAllFromDirectory(structsDirectory)){
            return nullptr;
        }
    }

    KernelBuilder kernelBuilder;
    if (kernelBuilder.loadAllFromDirectory(kernelFragmentsDirectory)){
        return nullptr;
    }


    std::string validationMessage;
    if (kernelBuilder.validateLoadedFragments(validationMessage)){
        std::fprintf(stderr,"%s\n", validationMessage.c_str());
        return nullptr;
    }
    std::printf("%s\n", validationMessage.c_str());

    return build(kernelBuilder, structCollector, PBO, config, platformID, deviceID);
}

std::string PhysicsProcessorBuilder::getErrorString(cl_int error){
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
