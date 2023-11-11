#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    
    this->device = device;
    this->queue = cl::CommandQueue(openCLContext, device);
    
    cl_int error;
    
    this->pbo_mem = clCreateFromGLBuffer(context(), CL_MEM_WRITE_ONLY, PBO, &error);
    
    clEnqueueAcquireGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
    
    this->voxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);
    this->size = 0;
}

PhysicsProcessor::~PhysicsProcessor(){
    clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
}

void PhysicsProcessor::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(config.simulationWidth, config.simulationHeight));
    queue.finish();
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    struct vector2D{
        cl_uint x;
        cl_uint y;
    } newVector2D;
    newVector2D.x = 0;
    newVector2D.y = 0;
    
    struct voxel{
        cl_uint substanceID;
        struct vector2D forceVector;
        // psisicStateVariables TODO next iteration
    } newVoxel;
    newVoxel.substanceID = substanceID;
    newVoxel.forceVector = newVector2D;
    
    ++size;
    
    this->voxels[config.simulationWidth * x + y] = newVoxel;
}

uint PhysicsProcessor::countVoxels(){
	return size;
}
