#ifndef _PHYSICSPROCESSOR_H
#define _PHYSICSPROCESSOR_H

#include "IPhysicsProcessor.h"

class PhysicsProcessor : public IPhysicsProcessor{
public:
    PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config);
    ~PhysicsProcessor() override;
    void generateFrame() override;
    void spawnVoxel(uint x, uint y, uint substanceID) override;
    uint countVoxels() override;

private:
    engineConfig config;
    //resources: need to think about it
    cl::Context context;
    cl::Kernel engine;
    cl::CommandQueue queue;
};

#endif
