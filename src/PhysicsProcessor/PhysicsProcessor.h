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
    engineConfig config;
    //resources: need to think about it
    cl::Context context;
    cl::Kernel engine;
    cl::CommandQueue queue;
    cl::Device device;
    cl_mem pbo_mem;
    cl::Buffer pbo_buff;
    cl::Buffer voxels;
    uint size;
};

#endif
