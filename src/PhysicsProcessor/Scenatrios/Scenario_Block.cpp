#include "Scenario_Block.h"

void Scenario_Block::init(){
    drawBox();
    uint onefourthHeight = (height>>1) - (int(height>>1)%8);
    uint onefourthWidth = (width>>1) - (int(width>>1)%8);
    physicsProcessor->spawnVoxelInArea((width>>1) - (onefourthWidth>>1), (height>>1) - (onefourthHeight>>1), onefourthHeight, onefourthHeight, substanceID);
}

void Scenario_Block::repeat(){

}
