#include "Scenario_NormalDistribution.h"

void Scenario_NormalDistribution::init(){
    drawBox();
    for (uint i = 0; i < width/4; i++){
        for (uint j = 0; j < 350; j++){
            physicsProcessor->spawnVoxel(i*4, j, 1);
        }
    }

    for (uint i = 0; i < 35; i++){
        for (uint j = 0; j < i; j++){
            physicsProcessor->spawnVoxelInArea((width>>1) + 9*j - 4.5*(i-1) - 6, (height>>3)*7 - 16*i, 8, 8, 1);
        }
    }
    physicsProcessor->spawnVoxelInArea((width>>1)-28, height-64, 8, 64, 1);
    physicsProcessor->spawnVoxelInArea((width>>1)+20, height-64, 8, 64, 1);
}

void Scenario_NormalDistribution::repeat(){
    physicsProcessor->spawnVoxelInArea((width>>1)-16, height-32, 32, 16, 3);
}
