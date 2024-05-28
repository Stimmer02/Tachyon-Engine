#ifndef PHYSICSPROCESSOR_H
#define PHYSICSPROCESSOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#include "../Headers.h"

#include <stdint.h>

class PhysicsProcessor{
    friend class PhysicsProcessorBuilder;
public:
    ~PhysicsProcessor();

    void spawnVoxel(uint32_t x, uint32_t y, uint32_t substanceID);
    void spawnVoxelsInArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t substanceID);
    uint32_t countVoxels();

    void generateFrame();

private:
    PhysicsProcessor(const uint32_t& engineSize);
    bool fallback;
    float* hostFallbackBuffer;
    void* openGLFallbackBuffer;
    std::array<size_t, 3> fallbackOrigin;
    std::array<size_t, 3> fallbackRegion;

    cl::NDRange globalWorkSize;
    cl::NDRange localWorkSize;
    cl::NDRange countVoxelsSize;

    cl::Context context;
    cl::Device device;
    cl::CommandQueue queue;

    cl::Kernel* engine;
    const uint32_t engineSize;

    cl::Kernel spawn_voxelKernel;
    cl::Kernel spawn_voxel_in_areaKernel;
    cl::Kernel count_voxelKernel;

    cl::Buffer* engineResources;//contains all the resources for the engine
    cl::Buffer engineConfig;//contains the configuration for the engine
    cl::Buffer countVoxelReturnValue;//used to return the count of voxels
    cl::Buffer countVoxelWorkMemory;//used during count_voxels kernel

    cl_mem TBOMemory;//contains allocated memory for the TBO
    cl::Image2D  TBOBuffer;//contains the TBO buffer

    std::vector<cl::Buffer*> allocatedGPUMemory;
};

#endif
