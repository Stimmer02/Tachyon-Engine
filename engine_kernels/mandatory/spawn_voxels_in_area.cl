void kernel spawn_voxels_in_area(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){
    uint globalID, IDX, IDY;
    IDX = x + get_global_id(0);
    IDY = y + get_global_id(1);
    globalID = IDY * config->simulationWidth + IDX;

    if (config->simulationWidth > IDX && config->simulationHeight > IDY){
        resources->voxels[globalID].forceVector.x = 0;
        resources->voxels[globalID].forceVector.y = 0;
        resources->voxels[globalID].substanceID = substanceID;
    }
}
