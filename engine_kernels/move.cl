void kernel move(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        if (thisVoxel.substanceID > 0){
            resources->endpointMap[global_ID] = global_ID;
        }
        return;
    }   

    private int cursorX = thisVoxel.targetCell % get_global_size(0);
    private int cursorY = thisVoxel.targetCell / get_global_size(0);


    if (resources->endpointMap[thisVoxel.targetCell] == global_ID){
        return;
    }

    private int error, doubleError;
    private bool tempLogic;

    int newVectorX = cursorX - get_global_id(0);
    int newVectorY = cursorY - get_global_id(1);

    private short normalVectorX = (newVectorX > 0.0f) ? -1 : 1; //reversed
    private short normalVectorY = (newVectorY > 0.0f) ? -1 : 1; //reversed
    private int absVectorX = abs(newVectorX);
    private int absVectorY = abs(newVectorY);
    private int loopLength = thisVoxel.loopLength;
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
            resources->voxels[global_ID].state = 2;
            resources->endpointMap[cursorX + cursorY * config->simulationWidth] = global_ID;
            return;
        }
    }

    // at this point, voxel cannot return to its target cell
    resources->missingVoxels[global_ID] = global_ID;
} 
