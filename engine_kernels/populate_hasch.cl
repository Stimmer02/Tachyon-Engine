void kernel populate_hash(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        return;
    }

    private int cursorX = get_global_id(0);
    private int cursorY = get_global_id(1);

    private int error, doubleError;
    private int current Cursor, previousCursor;
    private bool tempLogic;
    global struct voxel* currentVoxel; // voxel under cursor

    private short normalVectorX = (thisVoxel.forceVector.x > 0) ? 1 : -1;
    private short normalVectorY = (thisVoxel.forceVector.y > 0) ? 1 : -1;
    private int absVectorX = abs(thisVoxel.forceVector.x);
    private int absVectorY = abs(thisVoxel.forceVector.y);
    private int loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;

    resources->hashMap[global_ID] = global_ID;

    if (loopLength == 0){
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
        
        previousCursor = currentCursor;
        currentCursor = cursorX + cursorY * config->simulationWidth;


        if (cursorX >= config->simulationWidth || cursorY >= config->simulationHeight || cursorX < 0 || cursorY < 0){
            // getting out of bounds
            resources->voxels[global_ID].substanceID = 0;
            return;
        }

        currentVoxel = &resources->voxels[currentCursor];
        if (currentVoxel->substanceID > 0){
            // cursor hits other voxel's starting position
            if (resources->SUBSTANCES[currentVoxel->substanceID].movable == 0){
                // voxel is not movable
                resources->voxels[global_ID].forceVector.x = 0;
                resources->voxels[global_ID].forceVector.y = 0;
                resources->endpointMap[previousCursor] = global_ID;
                return;
            }
        }
        resources->hashMap[currentCursor] = global_ID;
    }
    resources->endpointMap[currentCursor] = global_ID;
}


