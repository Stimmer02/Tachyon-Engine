struct __attribute__ ((aligned)) engineResources{
    struct substance* SUBSTANCES;
    struct voxel* voxels; //ALLOCATE SIM_SIZE
    struct color* PBO;
};
