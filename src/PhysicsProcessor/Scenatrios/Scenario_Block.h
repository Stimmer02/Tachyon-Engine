#ifndef SCENARIO_BLOCK_H
#define SCENARIO_BLOCK_H

#include "AScenario.h"

class Scenario_Block : public AScenario{
public:
    Scenario_Block(IPhysicsProcessor* physicsProcessor, uint width, uint height, uint substanceID) : AScenario(physicsProcessor, width, height){
        this->substanceID = substanceID;
    };
    void init() override;
    void repeat() override;

private:
    uint substanceID;
};

#endif
