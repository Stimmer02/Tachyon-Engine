void kernel apply_flowability(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxelsCopy[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0){
        return;
    }

    if (thisVoxel.state == 0){
        return;
    }

    private float jammingFactor = resources->SUBSTANCES[thisVoxel.substanceID].JAMMING_FACTOR;

    if (jammingFactor > 0.75f){
        return;
    }

    private bool leftBorder = get_global_id(0) == 0;
    private bool rightBorder = get_global_id(0) == get_global_size(0) - 1;
    private bool bottomBorder = get_global_id(1) == 0;

    private bool moveRight = rightBorder || resources->voxelsCopy[global_ID + 1].substanceID == 0;
    private bool moveLeft = leftBorder || resources->voxelsCopy[global_ID - 1].substanceID == 0;
    
    // if (jammingFactor > 0.25f){
    //     // calculating if there is voxel below and to the left/right
    //     moveRight = moveRight && (bottomBorder || resources->voxelsCopy[global_ID + 1 + get_global_size(0)].substanceID == 0);
    //     moveLeft = moveLeft && (bottomBorder || resources->voxelsCopy[global_ID - 1 + get_global_size(0)].substanceID == 0);
    // }


    if (!(moveRight ^ moveLeft)){
        moveLeft = ((get_global_id(0) & 1)^(get_global_id(1) & 1));
        moveRight = !moveLeft;
    }

    resources->voxelsCopy[global_ID].forceVector.x = thisVoxel.forceVector.x + (2-jammingFactor)*(moveRight - moveLeft);
}