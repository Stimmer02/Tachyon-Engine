#ifndef SCENARIO_NORMALDISTRIBUTION_H
#define SCENARIO_NORMALDISTRIBUTION_H

#include "AScenario.h"

class Scenario_NormalDistribution : public AScenario{
public:
    Scenario_NormalDistribution(IPhysicsProcessor* physicsProcessor, uint width, uint height) : AScenario(physicsProcessor, width, height){};
    void init() override;
    void repeat() override;
};

#endif
