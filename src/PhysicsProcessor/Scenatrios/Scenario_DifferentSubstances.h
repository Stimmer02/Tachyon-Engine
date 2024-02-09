#ifndef SCENARIO_DIFFERENTSUBSTANCES_H
#define SCENARIO_DIFFERENTSUBSTANCES_H

#include "AScenario.h"

class Scenario_DifferentSubstances : public AScenario{
    public:
    Scenario_DifferentSubstances(IPhysicsProcessor* physicsProcessor, uint width, uint height) : AScenario(physicsProcessor, width, height){};
    void init() override;
    void repeat() override;
};

#endif
