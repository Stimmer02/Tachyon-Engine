void kernel populate_collision(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        return;
    }

    private int IDX, IDY;

    IDX = get_global_id(0);
    IDY = get_global_id(1);

    private int cursorX, cursorY;
    cursorX = IDX;
    cursorY = IDY;

    private int error, doubleError;
    private short normalVectorX, normalVectorY;
    private int absVectorX, absVectorY, loopLength;
    private bool tempLogic;


    normalVectorX = (thisVoxel.forceVector.x > 0) ? 1 : -1;
    normalVectorY = (thisVoxel.forceVector.y > 0) ? 1 : -1;
    absVectorX = abs(thisVoxel.forceVector.x);
    absVectorY = abs(thisVoxel.forceVector.y);
    loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;

    if (resources->hashMap[global_ID] != global_ID){
        resources->collisionMap[global_ID] = global_ID;
    }

    if (loopLength > 0){
        doubleError = error*2;

        //updating cursor position
        tempLogic = doubleError > -absVectorY;
        error -= absVectorY * tempLogic;
        cursorX += normalVectorX * tempLogic;

        tempLogic = doubleError < absVectorX;
        error += absVectorX * tempLogic;
        cursorY += normalVectorY * tempLogic;

        if (resources->hashMap[cursorX + cursorY * config->simulationWidth] != global_ID){
            resources->collisionMap[cursorX + cursorY * config->simulationWidth] = global_ID;
        }
        

        for (uint i = 1; i < loopLength; i++){
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
    } else {
        resources->endpointMap[global_ID] = global_ID;
    }
}
