void kernel populate_hash(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxels[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        return;
    }

    private int cursorX = get_global_id(0);
    private int cursorY = get_global_id(1);

    private int error, doubleError;
    private bool tempLogic;
    global struct voxel* currentVoxel; // voxel under cursor

    private short normalVectorX = ((int)thisVoxel.forceVector.x > 0.0f) ? 1 : -1;
    private short normalVectorY = ((int)thisVoxel.forceVector.y > 0.0f) ? 1 : -1;
    private int absVectorX = abs((int)(thisVoxel.forceVector.x));
    private int absVectorY = abs((int)(thisVoxel.forceVector.y));
    private int loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
    error = absVectorX - absVectorY;

    resources->hashMap[global_ID] = global_ID;

    if (loopLength == 0){
        thisVoxel.loopLength = 0;
        thisVoxel.targetCell = global_ID;
        resources->voxels[global_ID] = thisVoxel;
        return;
    }
    private int currentCursor = global_ID;
    private int previousCursor = global_ID;
    private int completlyUselessVariableThatTriesToResolveUterlyStupidCompilerError = global_ID;
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
                thisVoxel.loopLength = i;
                thisVoxel.targetCell = completlyUselessVariableThatTriesToResolveUterlyStupidCompilerError;
                thisVoxel.forceVector.x = 0.0f;
                thisVoxel.forceVector.y = 0.0f;
                resources->voxels[global_ID] = thisVoxel;
                resources->endpointMap[previousCursor] = global_ID;
                return;
            }
        }
        resources->hashMap[currentCursor] = global_ID;
        completlyUselessVariableThatTriesToResolveUterlyStupidCompilerError = currentCursor;
    }
    resources->endpointMap[currentCursor] = global_ID;
    thisVoxel.loopLength = loopLength;
    thisVoxel.targetCell = completlyUselessVariableThatTriesToResolveUterlyStupidCompilerError;
    resources->voxels[global_ID] = thisVoxel;
}


