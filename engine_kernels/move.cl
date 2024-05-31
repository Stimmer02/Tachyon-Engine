void kernel move(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        if (thisVoxel.substanceID > 0){
            resources->endpointMap[global_ID] = global_ID;
        }
        return;
    }
    
    private int cursorX, cursorY;

    cursorX = thisVoxel.forceVector.x + get_global_id(0);
    cursorY = thisVoxel.forceVector.y +  get_global_id(1);

    private int targetCell = cursorX + cursorY * config->simulationWidth;

    if (resources->endpointMap[targetCell] == global_ID){
        resources->endpointMap[targetCell] = global_ID;
        return;
    }

    private int error, doubleError;
    private short normalVectorX, normalVectorY;
    private int absVectorX, absVectorY, loopLength;
    private bool tempLogic;

    normalVectorX = (thisVoxel.forceVector.x > 0) ? -1 : 1; // reversed
    normalVectorY = (thisVoxel.forceVector.y > 0) ? -1 : 1;
    absVectorX = abs(thisVoxel.forceVector.x);
    absVectorY = abs(thisVoxel.forceVector.y);
    loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;


    for (uint i = 0; i < loopLength; i++){
        doubleError = error*2;

        tempLogic = doubleError > -absVectorY;
        error -= absVectorY * tempLogic;
        cursorX += normalVectorX * tempLogic;

        tempLogic = doubleError < absVectorX;
        error += absVectorX * tempLogic;
        cursorY += normalVectorY * tempLogic;

        if (resources->collisionMap[cursorX + cursorY * config->simulationWidth] == 0){

            // TODO: calculate side force based on the jamming factor
            thisVoxel.forceVector.x = 0;
            thisVoxel.forceVector.y = 0;
            resources->voxels[cursorX + cursorY * config->simulationWidth] = thisVoxel;
            resources->endpointMap[cursorX + cursorY * config->simulationWidth] = global_ID;
            return;
        }
    }
    // printf("Voxel %d has no place to go\n", global_ID);
    resources->missingVoxels[global_ID] = global_ID;
} 
