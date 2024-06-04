void kernel drown_voxel(global struct engineConfig* config, global struct engineResources* resources, write_only image2d_t TBO){
    private int global_ID = get_global_id(0) + get_global_id(1) * get_global_size(0);
    private struct voxel thisVoxel = resources->voxelsCopy[global_ID];

    if (resources->SUBSTANCES[thisVoxel.substanceID].movable == 0 || thisVoxel.forceVector.y > 0){
        return;
    }

    if (get_global_id(1) == get_global_size(1) - 1){
        return;
    }

    private struct voxel voxelBelow = resources->voxelsCopy[global_ID + get_global_size(1)];

    if (resources->SUBSTANCES[voxelBelow.substanceID].movable == 0){
        return;
    }

    if (resources->SUBSTANCES[thisVoxel.substanceID].MASS <= resources->SUBSTANCES[voxelBelow.substanceID].MASS){
        return;
    }

    if (resources->SUBSTANCES[voxelBelow.substanceID].JAMMING_FACTOR > 0.25f){
        return;
    }

    thisVoxel.forceVector.y = 0.0f;
    resources->voxelsCopy[global_ID] = voxelBelow;
    resources->voxelsCopy[global_ID + get_global_size(1)] = thisVoxel;
}
