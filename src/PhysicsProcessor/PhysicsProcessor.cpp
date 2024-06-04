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

    if( TBOBuffer() != 0 && fallback == false){
        clEnqueueReleaseGLObjects(queue(), 1, &TBOMemory, 0, NULL, NULL);
    }
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
    queue.enqueueNDRangeKernel(spawn_voxel_in_areaKernel, cl::NullRange, cl::NDRange(width, height));
    queue.finish();
}

uint32_t PhysicsProcessor::countVoxels(){
    cl_uint returnValue;
    queue.enqueueNDRangeKernel(count_voxelKernel, cl::NullRange, countVoxelsSize, countVoxelsSize);
    queue.enqueueReadBuffer(countVoxelReturnValue, CL_FALSE, 0, sizeof(cl_uint), &returnValue);
    queue.finish();

    return returnValue;
}

char PhysicsProcessor::loadSimulation(const std::string& path){
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()){
        return 1;
    }
    int width, height;
    file.read((char*)&width, sizeof(int));
    file.read((char*)&height, sizeof(int));

    if (width != globalWorkSize[0] || height != globalWorkSize[1]){
        return 2;
    }

    int* arr = new int[width * height];
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(int) * width * height);

    file.read((char*)arr, sizeof(int) * width * height);
    file.close();

    queue.enqueueWriteBuffer(buffer, CL_FALSE, 0, sizeof(int) * width * height, arr);
    load_simulationKernel.setArg(2, buffer);
    queue.enqueueNDRangeKernel(load_simulationKernel, cl::NullRange, globalWorkSize);
    queue.finish();


    delete[] arr;
    return 0;
}

char PhysicsProcessor::saveSimulation(const std::string& path){
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()){
        return 1;
    }

    int width = globalWorkSize[0];
    int height = globalWorkSize[1];

    file.write((char*)&width, sizeof(int));
    file.write((char*)&height, sizeof(int));

    int* arr = new int[width * height];
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(int) * width * height);

    save_simulationKernel.setArg(2, buffer);
    queue.enqueueNDRangeKernel(save_simulationKernel, cl::NullRange, globalWorkSize);
    queue.enqueueReadBuffer(buffer, CL_FALSE, 0, sizeof(int) * width * height, arr);
    cl_int clError = queue.finish();

    if (clError != CL_SUCCESS){
        return 2;
    }

    file.write((char*)arr, sizeof(int) * width * height);
    file.close();

    delete[] arr;
    return 0;
}

int PhysicsProcessor::getSumulationWidth(){
    return globalWorkSize[0];
}

int PhysicsProcessor::getSumulationHeight(){
    return globalWorkSize[1];
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
