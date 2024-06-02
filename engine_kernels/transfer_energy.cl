void kernel transfer_energy(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);

    private struct voxel thisVoxel = resources->voxelsCopy[global_ID];
    if (thisVoxel.state == 0){
        return;
    }

    thisVoxel.forceVector.x = 0;
    thisVoxel.forceVector.y = 0;
    thisVoxel.state = 0;

    resources->voxelsCopy[global_ID] = thisVoxel;
}
