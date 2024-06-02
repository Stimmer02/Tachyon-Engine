void kernel transfer_energy(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);

    private struct voxel thisVoxel = resources->voxelsCopy[global_ID];
    if (thisVoxel.state == 0){
        return;
    }

    private float energyTransfer = 1.0f - resources->SUBSTANCES[thisVoxel.substanceID].JAMMING_FACTOR;
    private char direction = 1 - 2 * ((get_global_id(0) & 1)^(get_global_id(1) & 1));

    thisVoxel.forceVector.x *= energyTransfer/2;
    thisVoxel.forceVector.x += thisVoxel.forceVector.y * energyTransfer * direction;

    thisVoxel.forceVector.y *= -0.14f;
    thisVoxel.state = 0;

    resources->voxelsCopy[global_ID] = thisVoxel;
}
