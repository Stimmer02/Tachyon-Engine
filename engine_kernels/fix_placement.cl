void kernel fix_placement(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);

    if (resources->missingVoxels[global_ID] == 0){
        return;
    }
    resources->missingVoxels[global_ID] = 0;

    private int cursor = global_ID;
    private int swap;
    global struct voxel* currentVoxel;
    private float reduceEnergy = 1.8;

    while (resources->endpointMap[cursor] != 0){
        swap = resources->endpointMap[cursor];
        resources->endpointMap[cursor] = cursor;
        currentVoxel = &resources->voxels[cursor];
        currentVoxel->forceVector.x /= reduceEnergy;
        currentVoxel->forceVector.y /= reduceEnergy;
        cursor = swap;
    }

    resources->endpointMap[cursor] = cursor;
    currentVoxel = &resources->voxels[cursor];
    currentVoxel->forceVector.x /= reduceEnergy;
    currentVoxel->forceVector.y /= reduceEnergy;
}
