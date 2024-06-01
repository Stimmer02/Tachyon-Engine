void kernel populate_collision(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        return;
    }

    private int cursorX = get_global_id(0);
    private int cursorY = get_global_id(1);

    private int error, doubleError;
    private bool tempLogic;

    private short normalVectorX = (thisVoxel.forceVector.x > 0) ? 1 : -1;
    private short normalVectorY = (thisVoxel.forceVector.y > 0) ? 1 : -1;
    private int absVectorX = abs(thisVoxel.forceVector.x);
    private int absVectorY = abs(thisVoxel.forceVector.y);
    private int loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;

    if (resources->hashMap[global_ID] != global_ID){
        resources->collisionMap[global_ID] = global_ID;
    }

    if (loopLength == 0){
        resources->endpointMap[global_ID] = global_ID;
        return;
    }

    for (uint i = 0; i < loopLength; i++){
        doubleError = error*2;

        tempLogic = doubleError > -absVectorY;
        error -= absVectorY * tempLogic;
        cursorX += normalVectorX * tempLogic;

        tempLogic = doubleError < absVectorX;
        error += absVectorX * tempLogic;
        cursorY += normalVectorY * tempLogic;


        if (resources->hashMap[cursorX + cursorY * config->simulationWidth] != global_ID){
            resources->collisionMap[cursorX + cursorY * config->simulationWidth] = global_ID;
        }
    }
}
