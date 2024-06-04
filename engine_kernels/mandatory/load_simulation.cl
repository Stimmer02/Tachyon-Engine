void kernel load_simulation(global struct engineResources* resources, global struct engineConfig* config, global int* substanceIDArray){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    resources->voxels[global_ID].substanceID = substanceIDArray[global_ID];
}