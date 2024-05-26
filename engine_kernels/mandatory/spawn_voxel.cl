void kernel spawn_voxel(uint x, uint y, uint substanceID, global struct engineResources* resources, global struct engineConfig* config){
    resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.x = 0;
    resources->worldMap->voxels[y * config->simulationWidth + x].forceVector.y = 0;
    resources->worldMap->voxels[y * config->simulationWidth + x].substanceID = substanceID;
}