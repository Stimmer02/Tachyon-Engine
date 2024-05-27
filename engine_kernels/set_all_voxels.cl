void kernel set_all_voxels(global struct engineConfig* config, global struct engineResources* resources){
    private int global_ID;

    global_ID = get_global_id(0) + get_global_id(1) * config->simulationWidth;
    resources->voxels[global_ID].substanceID = 1;
}