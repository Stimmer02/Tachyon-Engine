#ifndef _PHYSICSPROCESSOR_H
#define _PHYSICSPROCESSOR_H

#include "IPhysicsProcessor.h"
#ifdef __APPLE__
#include "../OpenCL/include/CL/cl.hpp"
#else
#include <CL/opencl.hpp>
#endif

class PhysicsProcessor : public IPhysicsProcessor{
public:
    void allocateHostMemory(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    std::string structuresAsString();
    std::string kernelCodeAsString();
    void constructorMain(cl::Context openCLContext, engineConfig config, cl::Device device);
    void configureMainKernel();
    PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    ~PhysicsProcessor();
    void generateFrame() override;
    void spawnVoxel(uint x, uint y, uint substanceID) override;
    uint countVoxels() override;

private:
    cl::Context context;
    cl::Kernel engine;
    engineConfig config;
    cl::Device device;

    std::vector<cl::Buffer*> allocatedGPUMemory;

    cl::CommandQueue queue;
    cl_mem pbo_mem;
    cl::Buffer pbo_buff;
    cl::Buffer engineResources;
    cl::Buffer eConfig;
    uint size;
    cl::Kernel spawn_voxelKernel;
};

#endif
