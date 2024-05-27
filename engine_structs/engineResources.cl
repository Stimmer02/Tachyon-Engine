struct __attribute__ ((packed)) engineResources{
    struct substance* SUBSTANCES;
    struct voxel* voxels; //ALLOCATE SIM_SIZE
    struct color* PBO;
};
