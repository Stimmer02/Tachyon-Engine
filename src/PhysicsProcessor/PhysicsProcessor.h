#ifndef _PHYSICSPROCESSOR_H
#define _PHYSICSPROCESSOR_H

#include <stdint.h>

#include "IPhysicsProcessor.h"
#ifdef __APPLE__
#include "../OpenCL/include/CL/cl.hpp"
#else
#include <CL/opencl.hpp>
#endif

class PhysicsProcessor : public IPhysicsProcessor{
public:
    void allocateHostMemory(cl::Context openCLContext, cl::Kernel engine, uint32_t texture, engineConfig config, cl::Device device);
    std::string structuresAsString();
    std::string kernelCodeAsString();
    void constructorMain(cl::Context openCLContext, engineConfig config, cl::Device device);
    void configureMainKernel();
    PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    ~PhysicsProcessor();
    void generateFrame() override;
    void spawnVoxel(uint x, uint y, uint substanceID) override;
    uint countVoxels() override;
    void spawnVoxelInArea(uint x, uint y, uint width, uint height, uint substanceID) override;


private:
    cl::Context context;
    cl::Kernel engine;
    engineConfig config;
    cl::Device device;

    std::vector<cl::Buffer*> allocatedGPUMemory;

    cl::CommandQueue queue;
    cl_mem pbo_mem;
    cl::Buffer engineResources;
    cl::Buffer eConfig;
    cl::Buffer sumReturnValue;
    cl::Kernel spawn_voxelKernel;
    cl::Kernel sum_voxelKernel;
    cl::Kernel spawn_voxel_in_areaKernel;

};

#endif
