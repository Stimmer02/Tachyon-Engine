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
    void spawnVoxel(int x, int y, int type);
    void spawnVoxelsInArea(int x, int y, int width, int height, int type);
    uint countVoxels();

    void generateFrame();

private:
    PhysicsProcessor(const uint& engineSize);

    cl::Context context;
    cl::Device device;
    cl::CommandQueue queue;

    cl::Kernel* engine;
    const uint engineSize; 

    cl::Kernel spawn_voxelKernel;
    cl::Kernel count_voxelKernel;
    cl::Kernel spawn_voxel_in_areaKernel;
};

#endif
