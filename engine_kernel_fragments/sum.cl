void kernel TACHYON_ENGINE(global struct engineConfig* config, global struct engineResources* resources){
    private int IDX, IDY, global_ID;
    local struct engineConfig localConfig;
    struct voxel* thisVoxel;
    private struct voxel thisVoxelCopy;

    IDX = get_global_id(0);
    IDY = get_global_id(1);

    localConfig = *config;
    global_ID = IDX + IDY * localConfig.simulationWidth;
    thisVoxel = &resources->worldMap->voxels[global_ID];
    thisVoxelCopy = *thisVoxel;

    if (thisVoxel->substanceID > 1){
        thisVoxelCopy.forceVector.y -= localConfig.gravity;

        private int tempX, tempY;
        tempX = IDX;
        tempY = IDY;

        private int error, doubleError;
        private short scalarVectorX, scalarVectorY;
        private int absVectorX, absVectorY, loopLength;
        private bool tempLogic;
        struct voxel* prevVoxel, *currentVoxel;

        prevVoxel = thisVoxel;

        scalarVectorX = (thisVoxelCopy.forceVector.x > 0) ? 1 : -1;
        scalarVectorY = (thisVoxelCopy.forceVector.y > 0) ? 1 : -1;
        absVectorX = abs(thisVoxelCopy.forceVector.x);
        absVectorY = abs(thisVoxelCopy.forceVector.y);
        loopLength = (absVectorX > absVectorY) ? absVectorX : absVectorY;
        error = absVectorX - absVectorY;



        if (loopLength > 0){
            doubleError = error*2;

            tempLogic = doubleError > -absVectorY;
            error -= absVectorY * tempLogic;
            tempX += scalarVectorX * tempLogic;

            tempLogic = doubleError < absVectorX;
            error += absVectorX * tempLogic;
            tempY += scalarVectorY * tempLogic;

            if (tempX >= localConfig.simulationWidth || tempY >= localConfig.simulationHeight || tempX <= 0 || tempY <= 0){
                prevVoxel->substanceID = 0;
            } else {
                currentVoxel = &resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth];
                if (currentVoxel->substanceID > 0){
                    prevVoxel->forceVector.x = 0;
                    prevVoxel->forceVector.y = 0;
                } else {
                    prevVoxel->substanceID = 0;
                    prevVoxel = currentVoxel;

                    for (uint i = 1; i < loopLength; i++){
                        doubleError = error*2;

                        tempLogic = doubleError > -absVectorY;
                        error -= absVectorY * tempLogic;
                        tempX += scalarVectorX * tempLogic;

                        tempLogic = doubleError < absVectorX;
                        error += absVectorX * tempLogic;
                        tempY += scalarVectorY * tempLogic;

                        if (tempX >= localConfig.simulationWidth || tempY >= localConfig.simulationHeight || tempX <= 0 || tempY <= 0){
                            thisVoxelCopy.substanceID = 0;
                            break;
                        }
                        currentVoxel = &resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth];
                        if (currentVoxel->substanceID > 0){
                            prevVoxel->substanceID = thisVoxelCopy.substanceID; //redundant but more safe
                            thisVoxelCopy.forceVector.x = 0;
                            thisVoxelCopy.forceVector.y = 0;
                            break;
                        }
                        prevVoxel = currentVoxel;
                    }
                    prevVoxel->substanceID = thisVoxelCopy.substanceID;
                    prevVoxel->forceVector.y = thisVoxelCopy.forceVector.y;
                    prevVoxel->forceVector.x = thisVoxelCopy.forceVector.x;
                }
            }
        }


        if (tempY > 0 && resources->worldMap->voxels[tempX + (tempY - 1) * localConfig.simulationWidth].substanceID > 0){

            private int iterL = 1000 * (1 - resources->substanceTable->substances[thisVoxelCopy.substanceID].jammingFactor);
            private int iterR = iterL;
            iterL = (0 > tempX - iterL) ? tempX : iterL;
            iterR = (localConfig.simulationWidth <= tempX + iterR) ? localConfig.simulationWidth - tempX : iterR;

            for (private uint i = 0; iterL | iterR; i++){
                if (iterL){
                    if (resources->worldMap->voxels[tempX - i + (tempY - 1) * localConfig.simulationWidth].substanceID == 0){
                        resources->worldMap->voxels[tempX - i + (tempY - 1) * localConfig.simulationWidth] = resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth];
                        resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth].substanceID = 0;
                        break;
                    } else if (resources->worldMap->voxels[tempX - i + (tempY - 1) * localConfig.simulationWidth].substanceID == thisVoxelCopy.substanceID){
                        iterL--;
                    } else {
                        iterL = 0;
                    }
                }
                if (iterR){
                    if (resources->worldMap->voxels[tempX + i + (tempY - 1) * localConfig.simulationWidth].substanceID == 0){
                        resources->worldMap->voxels[tempX + i + (tempY - 1) * localConfig.simulationWidth] = resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth];
                        resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth].substanceID = 0;
                        break;
                    } else if (resources->worldMap->voxels[tempX + i + (tempY - 1) * localConfig.simulationWidth].substanceID == thisVoxelCopy.substanceID){
                        iterR--;
                    } else {
                        iterR = 0;
                    }
                }
            }

            private struct substance substanceBelow = resources->substanceTable->substances[resources->worldMap->voxels[tempX + (tempY - 1) * localConfig.simulationWidth].substanceID];
            if (substanceBelow.mass < resources->substanceTable->substances[thisVoxelCopy.substanceID].mass && substanceBelow.jammingFactor){
                private struct voxel tempVoxel = resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth];
                resources->worldMap->voxels[tempX + tempY * localConfig.simulationWidth] = resources->worldMap->voxels[tempX + (tempY - 1) * localConfig.simulationWidth];
                resources->worldMap->voxels[tempX + (tempY - 1) * localConfig.simulationWidth] = tempVoxel;
            }
        }
        resources->PBO[tempX + tempY * localConfig.simulationWidth] = resources->substanceTable->substances[thisVoxelCopy.substanceID].color;
    }
    private uchar tempID;
    tempID = resources->worldMap->voxels[global_ID].substanceID;
    resources->PBO[global_ID] = resources->substanceTable->substances[tempID].color;
}


