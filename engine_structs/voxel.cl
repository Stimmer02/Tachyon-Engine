struct __attribute__ ((aligned(16))) voxel{
    unsigned int substanceID;
    int loopLength;
    int targetCell;
    struct vector2D forceVector;
};
