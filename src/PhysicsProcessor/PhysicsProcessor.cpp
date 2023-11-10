#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    
    this->device = getDefaultClDevice();
    this->queue = queue(openCLContext, default_device);
    
    cl_int error;
    
    this->pbo_mem = clCreateFromGLBuffer(context(), CL_MEM_WRITE_ONLY, PBO, &error);
    
    clEnqueueAcquireGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
    
    this->voxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * 4);
    this->size = 0;
    this->allocatedSize = 4; // TODO
}

PhysicsProcessor::~PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
}

void PhysicsProcessor::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(width, height), cl::NDRange(localXsize, localYsize));
    queue.finish();
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    struct vector2D{
        unsigned int x;
        unsigned int y;
    } newVector2D;
    newVector2D.x = x;
    newVector2D.y = y;
    
    struct voxel{
        unsigned int substanceID;
        struct vector2D forceVector;
        // psisicStateVariables TODO next iteration
    } newVoxel;
    newVoxel.substanceID = substanceID;
    newVoxel.forceVector = newVector;
    
    if (size < allocatedSize){
        this->voxels[size] = newVoxel;
        ++size;
    }
    else {
        struct voxel* newVoxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel)*allocatedSize);
        
        for (uint i = 0; i < allocatedSize; ++i){
            memcpy(&newVoxels[i], &voxels[i], sizeof(struct voxel));
        }
        
        this->voxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel)*allocatedSize*2);
        allocatedSize = allocatedSize * 2;
        
        for (uint i = 0; i < size; ++i){
            memcpy(&voxels[i], &newVoxels[i], sizeof(struct voxel));
        }
        
        this->voxels[size] = newVoxel;
        ++size;
    }
}

uint PhysicsProcessor::countVoxels(){
	return size;
}
