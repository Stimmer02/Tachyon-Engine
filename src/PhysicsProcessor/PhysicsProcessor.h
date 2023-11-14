#ifndef _PHYSICSPROCESSOR_H
#define _PHYSICSPROCESSOR_H

#include "IPhysicsProcessor.h"

class PhysicsProcessor : public IPhysicsProcessor{
public:
    PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device);
    ~PhysicsProcessor();
    void generateFrame() override;
    void spawnVoxel(uint x, uint y, uint substanceID) override;
    uint countVoxels() override;

private:
    //resources: need to think about it
    cl::Context context;
    cl::Kernel engine;
    engineConfig config;
    cl::Device device;

    cl::CommandQueue queue;
    cl_mem pbo_mem;
    cl::Buffer pbo_buff;
    cl::Buffer voxels;
    uint size;
    cl::Kernel spawn_voxelKernel;
};

#endif
