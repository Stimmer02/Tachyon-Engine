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

struct __attribute__ ((aligned)) vector2D{
    cl_uint x;
    cl_uint y;
};

struct __attribute__ ((aligned)) voxel{
    cl_uint substanceID;
    struct vector2D forceVector;
};

struct __attribute__ ((aligned)) chunk{
    struct voxel* voxels;
};

struct __attribute__ ((aligned)) substance{
    struct color color;
    float mass;
    float jammingFactor;
};

struct __attribute__ ((aligned)) engineResources{
    struct substanceTable* substanceTable;
    struct chunk* worldMap;
    struct color* PBO;
};

struct __attribute__ ((aligned)) engineConfig{
    cl_uint simulationWidth;
    cl_uint simulationHeight;
    float gravity;
    float timefactor;
    float atmosphereViscosity;
};

struct __attribute__ ((aligned)) substanceTable{
    struct substance* substances;
};

class IPhysicsProcessor{
public:
    ~IPhysicsProcessor(){};
    virtual void generateFrame() = 0;
    virtual void spawnVoxel(uint x, uint y, uint substanceID) = 0;
    virtual uint countVoxels() = 0;
};

#endif

#ifndef _ENGINECONFIG_H
#define _ENGINECONFIG_H

#include <CL/opencl.hpp>
typedef unsigned int uint;

struct engineConfig{
    uint simulationWidth;
    uint simulationHeight;
    float gravity;
    float timefactor;
    float atmosphereViscosity;
};

#endif
