#include <CL/opencl.hpp>
#include <cstdio>
#include <string>

cl::Context createClContext(cl_uint clPlatformID, cl_uint clDeviceID, std::string& clDeviceName, std::string& error);

int main(){
    std::string clDeviceName;
    std::string error;
    cl::Context context = createClContext(0, 0, clDeviceName, error);
    
    uint toAllocate = 1024*1024*12;
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, toAllocate);
    size_t size;
    cl_int status;
    status = buffer.getInfo(CL_MEM_SIZE, &size);
    if (status != CL_SUCCESS) {
        std::printf("Failed to get buffer size, error code: %d\n", status);
        return -1;
    }
    std::printf("Allocated %dB out of %dB\n", size, toAllocate);

    return 0;
}


cl::Context createClContext(cl_uint clPlatformID, cl_uint clDeviceID, std::string& clDeviceName, std::string& error){
    std::vector<cl::Platform> allPlatforms;
    cl::Platform::get(&allPlatforms);

    if (allPlatforms.empty()){
        error += "ERR: PhysicsProcessorBuilder::createClContext no platforms found\n";
        return NULL;
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
        return NULL;
    }

    if (allDevices.size() <= clDeviceID){
        error += "ERR: PhysicsProcessorBuilder::createClContext specified device does not exist (device 0 will be used)\n";
        clDeviceID = 0;
    }

    cl::Device device = allDevices[clDeviceID];
    clDeviceName = device.getInfo<CL_DEVICE_NAME>();

    cl::Context context(device);
    
    if (context() == NULL) {
        error += "ERR: PhysicsProcessorBuilder::createClContext failed to create OpenCL context for device: " + clDeviceName + " (NO ERROR SPECIFIED)\n";
        return NULL;
    }

    return context;
}
