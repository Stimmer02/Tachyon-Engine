#include "PhysicsProcessor.h"

PhysicsProcessor::PhysicsProcessor(const uint& engineSize): engineSize(engineSize){
    fallback = false;
    engine = new cl::Kernel[engineSize];
    engineResources = nullptr;
}

PhysicsProcessor::~PhysicsProcessor(){
    delete[] engine;

    // if (fallback){
    //     delete[] hostFallbackBuffer;
    // }

    for (cl::Buffer* buffer: allocatedGPUMemory){
        delete buffer;
    }

    if (engineResources != nullptr){
        delete engineResources;
    }
}

void PhysicsProcessor::spawnVoxel(uint x, uint y, uint substanceID){
    spawn_voxelKernel.setArg(0, x);
    spawn_voxelKernel.setArg(1, y);
    spawn_voxelKernel.setArg(2, substanceID);
    queue.enqueueNDRangeKernel(spawn_voxelKernel, cl::NullRange, cl::NDRange(1), cl::NDRange(1));
    queue.finish();
}

void PhysicsProcessor::spawnVoxelsInArea(uint x, uint y, uint width, uint height, uint substanceID){
    spawn_voxel_in_areaKernel.setArg(0, x);
    spawn_voxel_in_areaKernel.setArg(1, y);
    spawn_voxel_in_areaKernel.setArg(2, substanceID);
    queue.enqueueNDRangeKernel(spawn_voxel_in_areaKernel, cl::NullRange, cl::NDRange(width, height, 1), cl::NDRange(8, 8));
    queue.finish();
}

uint PhysicsProcessor::countVoxels(){
    cl_uint returnValue;
    queue.enqueueNDRangeKernel(count_voxelKernel, cl::NullRange, cl::NDRange(256), cl::NDRange(256));
    queue.enqueueReadBuffer(countVoxelReturnValue, CL_FALSE, 0, sizeof(cl_uint), &returnValue);
    queue.finish();

    return returnValue;
}

void PhysicsProcessor::generateFrame(){
    for (uint i = 0; i < engineSize; i++){
        queue.enqueueNDRangeKernel(engine[i], cl::NullRange, globalWorkSize, localWorkSize);
    }
    queue.finish();
}