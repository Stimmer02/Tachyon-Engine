void kernel create_new_voxel_map(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);

    private int voxelPreviousLocation = resources->endpointMap[global_ID];

    // this results in one pixel not being used
    if (voxelPreviousLocation == 0){
        return;
    }
    resources->endpointMap[global_ID] = 0;

    resources->voxelsCopy[global_ID] = resources->voxels[voxelPreviousLocation];

}
 
