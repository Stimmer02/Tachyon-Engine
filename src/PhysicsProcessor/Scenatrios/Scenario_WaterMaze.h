#ifndef SCENARIO_WATERMAZE_H
#define SCENARIO_WATERMAZE_H

#include "AScenario.h"

class Scenario_WaterMaze : public AScenario{
public:
    Scenario_WaterMaze(IPhysicsProcessor* physicsProcessor, uint width, uint height) : AScenario(physicsProcessor, width, height){};
    void init() override;
    void repeat() override;
};

#endif
