void kernel apply_gravity(global struct engineConfig* config, global struct engineResources* resources){
    private int global_ID;

    global_ID = get_global_id(0) + get_global_id(1) * config->simulationWidth;
    resources->worldMap->voxels[global_ID].forceVector.y -= config->gravity;
}