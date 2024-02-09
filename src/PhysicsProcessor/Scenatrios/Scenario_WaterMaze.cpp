#include "Scenario_WaterMaze.h"

void Scenario_WaterMaze::init(){
    drawBox();
    physicsProcessor->spawnVoxelInArea(0, height*0.8, width*0.9 - (int(width*0.9)%8), 8, 1);
    physicsProcessor->spawnVoxelInArea(width*0.8 - (int(width*0.8)%8), height*0.8, 8, 16, 1);
    physicsProcessor->spawnVoxelInArea(0, height*0.6, (width>>1) - 8, 8, 1);
    physicsProcessor->spawnVoxelInArea((width>>1) + 16, height*0.6, (width>>1) - 8, 8, 1);
    physicsProcessor->spawnVoxelInArea((width>>2), height*0.4, (width>>1)+24, 8, 1);
    physicsProcessor->spawnVoxelInArea(width*0.1, height*0.2, width*0.9 - (int(width*0.9)%8), 8, 1);
    // physicsProcessor->spawnVoxelInArea(width*0.1, height*0.2, 8, 16, 1);
}

void Scenario_WaterMaze::repeat(){
    physicsProcessor->spawnVoxelInArea(8, height*0.9 + 32, 8*4, 8, 4);
}
