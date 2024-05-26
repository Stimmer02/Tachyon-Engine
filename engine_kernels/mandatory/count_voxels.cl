void kernel count_voxels(global struct engineResources* resources, global uint* workArr, uint size, global uint* returnValue){
    private uint id = get_global_id(0);
    private uint dim = get_local_size(0);
    private char dividionRest = size & 0x1;
    private uint currentSize = size >> 1;
    private uint index;

    for (private uint i = id; i < currentSize; i += dim){
        index = i << 1;
        workArr[i] = (resources->worldMap->voxels[index].substanceID > 0) + (resources->worldMap->voxels[index+1].substanceID > 0);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    if (dividionRest){
        if (id == 0){
            workArr[currentSize] = (resources->worldMap->voxels[currentSize << 1].substanceID > 0);
        }
        currentSize++;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    dividionRest = currentSize & 0x1;
    currentSize >>= 1;

    while (currentSize > 1){
        for (private uint i = id; i < currentSize; i += dim){
            index = i << 1;
            workArr[i] = workArr[index] + workArr[index+1];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        if (dividionRest){
            if (id == 0){
                workArr[currentSize] = workArr[currentSize << 1];
            }
            currentSize++;
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        dividionRest = currentSize & 0x1;
        currentSize >>= 1;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    if (id == 0){
        *returnValue = workArr[0] + workArr[1];
        if (dividionRest == 1){
            *returnValue += workArr[2];
        }
    }
}