#ifndef _PHYSICSPROCESSOR_FALLBACK_H
#define _PHYSICSPROCESSOR_FALLBACK_H

#include "IPhysicsProcessor.h"
#ifdef __APPLE__
#include "../OpenCL/include/CL/cl.hpp"
#else
#include <CL/opencl.hpp>
#endif


class PhysicsProcessor_Fallback : public IPhysicsProcessor{
public:
    void allocateHostMemory(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    std::string structuresAsString();
    std::string kernelCodeAsString();
    PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    ~PhysicsProcessor_Fallback();
    void generateFrame() override;
    void spawnVoxel(uint x, uint y, uint substanceID) override;
    uint countVoxels() override;

private:
    //resources: need to think about it
    cl::Context context;
    cl::Kernel engine;
    engineConfig config;
    cl::Device device;

    std::vector<cl::Buffer*> allocatedGPUMemory;

    cl::CommandQueue queue;
    unsigned char* hostFallbackBuffer;
    cl_mem pbo_mem;
    cl::Buffer pbo_buff;
    cl::Buffer engineResources;
    cl::Buffer eConfig;
    uint size;
    cl::Kernel spawn_voxelKernel;
};

#endif
