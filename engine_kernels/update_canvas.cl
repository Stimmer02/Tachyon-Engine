void kernel update_canvas(global struct engineConfig* config, global struct engineResources* resources){
    private int global_ID;
    private uchar tempID;

    global_ID = get_global_id(0) + get_global_id(1) * config->simulationWidth;
    tempID = resources->voxels[global_ID].substanceID;
    resources->PBO[global_ID] = resources->SUBSTANCES[tempID].color;
}