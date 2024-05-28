#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(const uint32_t& engineSize): engineSize(engineSize){
    fallback = false;
    engine = new cl::Kernel[engineSize];
    engineResources = nullptr;
    hostFallbackBuffer = nullptr;
}

PhysicsProcessor::~PhysicsProcessor(){
    delete[] engine;

    if (fallback){
        delete[] hostFallbackBuffer;
    }

    for (cl::Buffer* buffer: allocatedGPUMemory){
        delete buffer;
    }

    if (engineResources != nullptr){
        delete engineResources;
    }

    if( TBOBuffer() != 0)
        clEnqueueReleaseGLObjects(queue(), 1, &TBOMemory, 0, NULL, NULL);
}

void PhysicsProcessor::spawnVoxel(uint32_t x, uint32_t y, uint32_t substanceID){
    spawn_voxelKernel.setArg(0, x);
    spawn_voxelKernel.setArg(1, y);
    spawn_voxelKernel.setArg(2, substanceID);
    queue.enqueueNDRangeKernel(spawn_voxelKernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
    queue.finish();
}

void PhysicsProcessor::spawnVoxelsInArea(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t substanceID){
    spawn_voxel_in_areaKernel.setArg(0, x);
    spawn_voxel_in_areaKernel.setArg(1, y);
    spawn_voxel_in_areaKernel.setArg(2, substanceID);
    queue.enqueueNDRangeKernel(spawn_voxel_in_areaKernel, cl::NullRange, cl::NDRange(width, height, 1), cl::NDRange(8, 8));
    queue.finish();
}

uint32_t PhysicsProcessor::countVoxels(){
    cl_uint returnValue;
    queue.enqueueNDRangeKernel(count_voxelKernel, cl::NullRange, countVoxelsSize, countVoxelsSize);
    queue.enqueueReadBuffer(countVoxelReturnValue, CL_FALSE, 0, sizeof(cl_uint), &returnValue);
    queue.finish();

    return returnValue;
}

void PhysicsProcessor::generateFrame(){
    for (uint32_t i = 0; i < engineSize; i++){
        queue.enqueueNDRangeKernel(engine[i], cl::NullRange, globalWorkSize, localWorkSize);
    }
    queue.finish();
    if (fallback){
        queue.enqueueReadImage(TBOBuffer, CL_TRUE, fallbackOrigin, fallbackRegion, 0, 0, hostFallbackBuffer);
    
        glBindTexture(GL_TEXTURE_2D, TBO);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fallbackRegion[0] , fallbackRegion[1], GL_RGBA, GL_FLOAT, hostFallbackBuffer);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
