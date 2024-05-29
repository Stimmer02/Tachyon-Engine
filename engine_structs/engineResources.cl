struct __attribute__ ((aligned(16))) engineResources{
    global struct substance* SUBSTANCES;
    global struct voxel* voxels; //ALLOCATE SIM_SIZE
    global uint* hashMap; //ALLOCATE SIM_SIZE
    global uint* collisionMap; //ALLOCATE SIM_SIZE
};
