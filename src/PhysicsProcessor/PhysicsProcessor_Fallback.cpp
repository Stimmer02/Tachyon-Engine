#include "PhysicsProcessor_Fallback.h"

PhysicsProcessor_Fallback::PhysicsProcessor_Fallback(cl::Context openCLContext, cl::Kernel engine, GLuint PBO, engineConfig config, cl::Device device){
    this->context = openCLContext;
    this->engine = engine;
    this->config = config;
    
    this->device = device;
    this->queue = cl::CommandQueue(openCLContext, device);
    
    this->pbo_mem = clCreateBuffer(context(), CL_MEM_WRITE_ONLY, sizeof(color) * config.simulationWidth * config.simulationHeight, NULL, NULL);
    this->hostFallbackBuffer = new unsigned char[sizeof(color) * config.simulationWidth * config.simulationHeight];
    
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
    
    this->voxels = cl::Buffer buffer_dims(openCLContext, CL_MEM_READ_WRITE, sizeof(struct voxel) * config.simulationWidth * config.simulationHeight);
    this->size = 0;
}

PhysicsProcessor_Fallback::~PhysicsProcessor_Fallback(){
    delete[] hostFallbackBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PhysicsProcessor_Fallback::generateFrame(){
    queue.enqueueNDRangeKernel(create_gradient, cl::NullRange, cl::NDRange(config.simulationWidth, config.simulationHeight), cl::NDRange(config.simulationWidth, config.simulationHeight));
    queue.enqueueReadBuffer(pbo_buff, CL_FALSE, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer, NULL, NULL);
    queue.finish();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(color) * config.simulationWidth * config.simulationHeight, hostFallbackBuffer);
}

void PhysicsProcessor_Fallback::spawnVoxel(uint x, uint y, uint substanceID){
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

uint PhysicsProcessor_Fallback::countVoxels(){
	return size;
}
