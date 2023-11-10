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
}

PhysicsProcessor::~PhysicsProcessor(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    clEnqueueReleaseGLObjects(queue(), 1, &pbo_mem, 0, NULL, NULL);
}

void PhysicsProcessor::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(width, height), cl::NDRange(localXsize, localYsize));
    queue.finish();
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    // TODO nezt iteration
}

uint PhysicsProcessor::countVoxels(){
    // TODO nezt iteration
}
