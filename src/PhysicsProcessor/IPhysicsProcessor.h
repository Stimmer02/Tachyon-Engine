#ifndef _IPHYSICSPROCESSOR_H
#define _IPHYSICSPROCESSOR_H

#define CL_HPP_TARGET_OPENCL_VERSION 200

#ifdef APPLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>
#include "OpenCL/include/CL/cl_gl.h"
#include "OpenCL/include/CL/cl.hpp"

#else

#include <CL/opencl.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <CL/cl_gl.h>
#include <GL/glx.h>

#endif

#ifndef _COLOR_H
#ifndef COLOR_H
struct __attribute__ ((packed)) color{
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;
};
#endif
#endif

#ifndef _VECTOR2D_H
#ifndef VECTOR2D_H
struct __attribute__ ((aligned)) vector2D{
    cl_uint x;
    cl_uint y;
};
#endif
#endif

#ifndef _VOXEL_H
#ifndef VOXEL_H
struct __attribute__ ((aligned)) voxel{
    cl_uint substanceID;
    struct vector2D forceVector;
    // psisicStateVariables TODO next iteration
};
#endif
#endif

#ifndef _CHUNK_H
#ifndef CHUNK_H
struct __attribute__ ((aligned)) chunk{
    struct voxel* voxels;
};
#endif
#endif

#include "engineConfig.h"

class IPhysicsProcessor{
public:
    ~IPhysicsProcessor(){};
    virtual void generateFrame() = 0;
    virtual void spawnVoxel(uint x, uint y, uint substanceID) = 0;
    virtual uint countVoxels() = 0;
};

#endif
