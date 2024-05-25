#ifndef PHYSICSPROCESSOR_H
#define PHYSICSPROCESSOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#ifdef __APPLE__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include <OpenCL/opencl.h>
#include <OpenCL/cl_gl.h>
#include "../OpenCL/include/CL/cl.hpp"

#elif __WIN32__

typedef unsigned int uint;

#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <CL/opencl.hpp>
#include <CL/cl_gl.h>

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
// #include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif


class PhysicsProcessor{
    friend class PhysicsProcessorBuilder;
public:
    ~PhysicsProcessor();

    void spawnVoxel(uint x, uint y, uint substanceID);
    void spawnVoxelsInArea(uint x, uint y, uint width, uint height, uint substanceID);
    uint countVoxels();

    void generateFrame();

private:
    PhysicsProcessor(const uint& engineSize);

    cl::NDRange globalWorkSize; //TODO: rethink this
    cl::NDRange localWorkSize;

    cl::Context context;
    cl::Device device;
    cl::CommandQueue queue;

    cl::Kernel* engine;
    const uint engineSize; 

    cl::Kernel spawn_voxelKernel;
    cl::Kernel spawn_voxel_in_areaKernel;
    cl::Kernel count_voxelKernel;

    cl::Buffer engineResources;//contains all the resources for the engine
    cl::Buffer engineConfig;//contains the configuration for the engine
    cl::Buffer countVoxelReturnValue;//used to return the count of voxels
    cl::Buffer countVoxelWorkMemory;//used during count_voxels kernel
};

#endif
