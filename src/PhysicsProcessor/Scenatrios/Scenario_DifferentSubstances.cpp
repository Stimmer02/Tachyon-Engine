#include "Scenario_DifferentSubstances.h"

void Scenario_DifferentSubstances::init(){
    drawBox();
    physicsProcessor->spawnVoxelInArea(0, width/3, height, 8, 1);
    physicsProcessor->spawnVoxelInArea(0, width/3*2, height, 8, 1);
}

void Scenario_DifferentSubstances::repeat(){
    physicsProcessor->spawnVoxelInArea((width>>1)-16, (height>>1) + (height>>4), 32, 8, 2);
    physicsProcessor->spawnVoxelInArea((width>>1)-16, (height>>1) - height/3 + (height>>4), 32, 8, 3);
    physicsProcessor->spawnVoxelInArea((width>>1)-16, (height>>1) + height/3 + (height>>4), 32, 8, 4);

}
