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
        std::fprintf(stderr ,"Error building code: %d\n", buildCode);
        std::fprintf(stderr ,"Trying fallback settings...\n");

        context = cl::Context(defaultDevice);
        program = cl::Program(context, sources);

        buildCode = program.build();
        if (buildCode != CL_SUCCESS) {
            std::fprintf(stderr ,"Error building code: %d\n", buildCode);
            std::fprintf(stderr ,"Error building content:\n%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(defaultDevice).c_str());
            std::fprintf(stderr ,"ERROR: not able to compile kernel!\n");
            return nullptr;
        }
        std::fprintf(stderr ,"WARNING: entering fallback mode\n");
        std::printf("Compillation successful!\n");
        cl::Kernel TACHYON_ENGINE(program, "TACHYON_ENGINE");
        return PhysicsProcesor = new PhysicsProcessor_Fallback(context, TACHYON_ENGINE, PBO, config, defaultDevice);
    }


    cl::Kernel TACHYON_ENGINE(program, "TACHYON_ENGINE");
    return PhysicsProcesor = new PhysicsProcessor(context, TACHYON_ENGINE, PBO, config, defaultDevice);
}

IPhysicsProcessor* PhysicsProcessorBuilder::build(const std::string kernelFragmentsDirectory, const std::string structsDirectory, GLuint PBO, engineConfig config, uint platformID, uint deviceID){

    StructCollector structCollector;
    if (structsDirectory.empty() == false){
        structCollector.loadAllFromDirectory(structsDirectory);
    }

    KernelBuilder kernelBuilder;
    kernelBuilder.loadAllFromDirectory(kernelFragmentsDirectory);
    std::string validationMessage;
    if (kernelBuilder.validateLoadedFragments(validationMessage)){
        std::fprintf(stderr,"%s\n", validationMessage.c_str());
        return nullptr;
    }
    std::printf("%s\n", validationMessage.c_str());

    return build(kernelBuilder, structCollector, PBO, config, platformID, deviceID);
}
