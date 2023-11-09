#ifndef _PHYSICSPROCESSOR_FALLBACK_H
#define _PHYSICSPROCESSOR_FALLBACK_H

#include "IPhysicsProcessor.h"


class PhysicsProcessor_Fallback : public IPhysicsProcessor{
public:
    PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config);
    ~PhysicsProcessor_Fallback() override;
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