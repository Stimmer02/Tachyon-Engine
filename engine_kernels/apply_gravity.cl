void kernel apply_gravity(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);

    resources->voxels[global_ID].forceVector.y += config->gravity;
}