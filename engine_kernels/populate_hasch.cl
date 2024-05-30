void kernel populate_hash(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
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
    private int absVectorX, absVectorY, loopLength, currentCursor, previousCursor;
    private bool tempLogic;
    global struct voxel* currentVoxel; // voxel under cursor


    normalVectorX = (thisVoxel.forceVector.x > 0) ? 1 : -1;
    normalVectorY = (thisVoxel.forceVector.y > 0) ? 1 : -1;
    absVectorX = abs(thisVoxel.forceVector.x);
    absVectorY = abs(thisVoxel.forceVector.y);
    loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;

    resources->hashMap[global_ID] = global_ID;

    if (loopLength > 0){
        doubleError = error*2;

        //updating cursor position
        tempLogic = doubleError > -absVectorY;
        error -= absVectorY * tempLogic;
        cursorX += normalVectorX * tempLogic;

        tempLogic = doubleError < absVectorX;
        error += absVectorX * tempLogic;
        cursorY += normalVectorY * tempLogic;

        currentCursor = cursorX + cursorY * config->simulationWidth;


        if (cursorX >= (int)config->simulationWidth || cursorY >= (int)config->simulationHeight || cursorX < 0 || cursorY < 0){
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
                resources->endpointMap[global_ID] = global_ID;
                return;
            }
        }

        resources->hashMap[currentCursor] = global_ID;

        for (uint i = 1; i < loopLength; i++){
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
}


