#ifndef ASCENARIO_H
#define ASCENARIO_H

#include "../IPhysicsProcessor.h"

class AScenario{
public:
    virtual void init() = 0;
    virtual void repeat() = 0;
protected:
    AScenario(IPhysicsProcessor* physicsProcessor, uint width, uint height){
        this->physicsProcessor = physicsProcessor;
        this->width = width;
        this->height = height;
    }
    void drawBox(){
        physicsProcessor->spawnVoxelInArea(0, 0, 8, width, 1);
        physicsProcessor->spawnVoxelInArea(0, 0, height, 8, 1);
        physicsProcessor->spawnVoxelInArea(height-8, 0, 8, width, 1);
        physicsProcessor->spawnVoxelInArea(0, width-8, height, 8, 1);
    }

    IPhysicsProcessor* physicsProcessor;
    uint width;
    uint height;
};

#endif
