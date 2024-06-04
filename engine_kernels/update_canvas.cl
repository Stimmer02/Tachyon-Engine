void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private int2 coord = (int2)(get_global_id(0), get_global_id(1));
    private struct voxel thisVoxel = resources->voxelsCopy[global_ID];
    resources->voxels[global_ID] = thisVoxel;
    resources->voxelsCopy[global_ID].substanceID = 0;

    write_imagef(TBO, coord, *((global float4*)(&resources->SUBSTANCES[thisVoxel.substanceID].color)));
}
