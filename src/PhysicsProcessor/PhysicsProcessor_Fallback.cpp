#include "PhysicsProcessor_Fallback.h"

PhysicsProcessor::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    this->openCLContext = openCLContext;
    this->engine = engine;
    this->config = config;
    
    this->device = getDefaultClDevice();
    this->queue = queue(openCLContext, default_device);
    
    this->pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color)*3840*2160, NULL, NULL); // TODO
    this->hostFallbackBuffer = new unsigned char[sizeof(color)*3840*2160];
    
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
}

PhysicsProcessor::~PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config){
    delete[] hostFallbackBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PhysicsProcessor_Fallback::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(width, height), cl::NDRange(localXsize, localYsize));
    queue.enqueueReadBuffer(pbo_buff, CL_FALSE, 0, sizeof(color)*width*height, hostFallbackBuffer, NULL, NULL);
    queue.finish();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color)*width*height, hostFallbackBuffer);
}

void PhysicsProcessor_Fallback::spawnVoxel(uint x, uint y, uint substanceID){
    // TODO nezt iteration
}

uint PhysicsProcessor_Fallback::countVoxels(){
    // TODO nezt iteration
}